const fs = require('fs');

exports.deleteFile = function(fname, callback) {
    fs.stat(fname, (err, stats) => {
        if (err)
          callback(new Error(`the file ${fname} does not exist`));
        else {
            fs.unlink(fname, err => {
                if (err) callback(new Error(`Could not delete ${fname}`));
                else callback();
            });
        }
    });
}
