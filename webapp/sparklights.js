var base_url = '';
    var access_token = '';

    setColor = function(r, g, b) {
      data = '20,1,1,1|'+r+','+g+','+b;
      $.post(base_url, {'access_token': window.access_token, 'args': data}, function(responseText, status) {
          if (status === 'success') {
             $('#notification-bar').text('Request sent');
          } else {
             $('#notification-bar').text('An error occurred');
          }
       });
    };

    $(document).ready(function() {
      $.getScript('access_token.js', function(data, textStatus, r) {
        base_url = 'https://api.spark.io/v1/devices/'+config.device_id+'/c';
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

      $('#btn-strobe').mousedown(function(event) {
        event.preventDefault();
        setColor(99, 99, 99);
      });

      $('#btn-strobe').mouseup(function(event) {
        event.preventDefault();
        setColor(0, 0, 0);
      });

      $("#colorpicker").spectrum({
          color: "#f00",
          flat: true,
          change: function(color) {
            r = Math.floor(color._r * 255.0 / 99.0);
            g = Math.floor(color._g * 255.0 / 99.0);
            b = Math.floor(color._b * 255.0 / 99.0);

            setColor(r, g, b);
          }
      });

    });