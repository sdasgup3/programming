var express = require('express'),
    app = express();

app.use(express.static(__dirname + '/Users'));

app.listen(8080);
