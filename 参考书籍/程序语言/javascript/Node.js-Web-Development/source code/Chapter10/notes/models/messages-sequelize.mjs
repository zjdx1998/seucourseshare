import Sequelize from 'sequelize';
import jsyaml    from 'js-yaml'; 
import fs        from 'fs-extra'; 
import util      from 'util'; 
import EventEmitter from 'events'; 
 
class MessagesEmitter extends EventEmitter {} 

import DBG from 'debug';
const debug = DBG('notes:model-messages'); 
const error = DBG('notes:error-messages'); 
 
var SQMessage; 
var sequlz; 

export const emitter = new MessagesEmitter();

async function connectDB() { 

    if (typeof sequlz === 'undefined') {
        const yamltext = await fs.readFile(process.env.SEQUELIZE_CONNECT, 'utf8');
        const params = jsyaml.safeLoad(yamltext, 'utf8'); 
        sequlz = new Sequelize(params.dbname, 
                        params.username, params.password, 
                        params.params); 
    }

    if (SQMessage) return SQMessage.sync(); 
        
    SQMessage = sequlz.define('Message', { 
        id: { type: Sequelize.INTEGER, autoIncrement: true, 
                                        primaryKey: true }, 
        from: Sequelize.STRING, 
        namespace: Sequelize.STRING, 
        message: Sequelize.STRING(1024), 
        timestamp: Sequelize.DATE 
    }); 
    return SQMessage.sync(); 
}

export async function postMessage(from, namespace, message) { 
    const SQMessage = await connectDB();
    const newmsg = await SQMessage.create({ 
        from, namespace, message, timestamp: new Date() 
    }); 
    var toEmit = { 
        id: newmsg.id, 
        from: newmsg.from, 
        namespace: newmsg.namespace, 
        message: newmsg.message, 
        timestamp: newmsg.timestamp 
    }; 
    emitter.emit('newmessage', toEmit); 
}

export async function destroyMessage(id, namespace) { 
    const SQMessage = await connectDB();
    const msg = await SQMessage.find({ where: { id } });
    if (msg) {
        msg.destroy();
        emitter.emit('destroymessage', { id, namespace }); 
    }
}

export async function recentMessages(namespace) { 
    const SQMessage = await connectDB();
    const messages = SQMessage.findAll({ 
        where: { namespace }, 
        order: [ 'timestamp' /*, 'DESC' */ ], 
        limit: 20 
    });
    return messages.map(message => { 
        return { 
            id: message.id, 
            from: message.from, 
            namespace: message.namespace, 
            message: message.message, 
            timestamp: message.timestamp 
        }; 
    }); 
}