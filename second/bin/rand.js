var FSO = new ActiveXObject('Scripting.FileSystemObject');
var p = FSO.CreateTextFile('numbers.txt', 2, 0);
for (i = 0; i < 100000; i++)
  p.WriteLine(Math.floor(Math.random() * 1114112).toString(16));