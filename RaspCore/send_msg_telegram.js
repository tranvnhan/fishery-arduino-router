process.env.NTBA_FIX_350 = true
var args = process.argv.slice(2);

const TelegramBot = require('node-telegram-bot-api');
const token = '***';
const bot = new TelegramBot(token, {polling: true});
const chatId = '***';
//bot.sendPhoto(chatId, 'i0000.jpg').then( ()=> process.exit())
//bot.sendMediaGroup(chatId, [ {type: 'photo', media: 'attach://home/ts/Desktop/lab8/i0000.jpg', filepath: true}, {type: 'photo', media: 'attach://home/ts/Desktop/lab8/i0001.jpg', filepath: true} ]).then( ()=> process.exit())
bot.sendPhoto(chatId, args[0]).then( ()=> process.exit())

