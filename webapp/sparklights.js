var base_url = '';
    var access_token = '';

    setColor = function(r, g, b) {
      data = '20,1,0,1|'+r+','+g+','+b;
      $.post(base_url + '/c', {'access_token': window.access_token, 'args': data}, function(responseText, status) {
          if (status === 'success') {
             $('#notification-bar').text('Request sent');
          } else {
             $('#notification-bar').text('An error occurred');
          }
       });
    };

    setSpeed = function(speed) {
      data = speed;
      $.post(base_url + '/delay', {'access_token': window.access_token, 'args': data}, function(responseText, status) {
          if (status === 'success') {
             $('#notification-bar').text('Request sent');
          } else {
             $('#notification-bar').text('An error occurred');
          }
       });
    };

    setBlues = function(speed) {
        if(!speed) speed = 0;
      data = '20,1,' + speed + ',2|0,0,50,20,0,20';
      $.post(base_url + '/c', {'access_token': window.access_token, 'args': data}, function(responseText, status) {
          if (status === 'success') {
             $('#notification-bar').text('Request sent');
          } else {
             $('#notification-bar').text('An error occurred');
          }
       });
    };

    setColorful = function(speed) {
        if(!speed) speed = 0
      data = '20,1,' + speed + ',6|0,0,50,20,0,20,50,0,0,30,20,0,0,40,0,0,20,20';
      $.post(base_url + '/c', {'access_token': window.access_token, 'args': data}, function(responseText, status) {
          if (status === 'success') {
             $('#notification-bar').text('Request sent');
          } else {
             $('#notification-bar').text('An error occurred');
          }
       });
    };

    $(document).ready(function() {
      $.getScript('access_token.js', function(data, textStatus, r) {
        base_url = 'https://api.spark.io/v1/devices/'+config.device_id;
        access_token = config.access_token;
      });

      $('#btn-blackout').click(function(event) {
        event.preventDefault();
        setColor(0, 0, 0);
      });

      $('#btn-white').click(function(event) {
        event.preventDefault();
        setColor(99, 99, 99);
      });

      $('#btn-blues').click(function(event) {
        event.preventDefault();
        setBlues(10);
      });

      $('#btn-colorful').click(function(event) {
        event.preventDefault();
        setColorful(10);
      });

      $('#btn-strobe').mousedown(function(event) {
        event.preventDefault();
        setColor(99, 99, 99);
      });

      $('#btn-strobe').mouseup(function(event) {
        event.preventDefault();
        setColor(0, 0, 0);
      });

      $('#btn-speedparty').click(function(event) {
        event.preventDefault();
        setSpeed(1);
      });

      $('#btn-speedfast').click(function(event) {
        event.preventDefault();
        setSpeed(4);
      });

      $('#btn-speedmed').click(function(event) {
        event.preventDefault();
        setSpeed(20);
      });

      $('#btn-speedslow').click(function(event) {
        event.preventDefault();
        setSpeed(80);
      });

      $('#btn-speedxslow').click(function(event) {
        event.preventDefault();
        setSpeed(1000);
      });

      $("#colorpicker").spectrum({
          color: "#f00",
          flat: true,
          change: function(color) {
            r = Math.floor(color._r * 100.0 / 255.0);
            g = Math.floor(color._g * 100.0 / 255.0);
            b = Math.floor(color._b * 100.0 / 255.0);

            setColor(r, g, b);
          }
      });

    });
