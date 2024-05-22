function fetchData() {
    $.ajax({
        type: 'POST',
        url: '/dashboard/devices_config',
        contentType: 'application/json',
        success: function(response) {
            if(response.data){
                console.log(response.data);
                response.data.forEach(element => {
                    if(element===undefined)
                        return;
                    var url='/content/devices_config/'+element;
                    $.ajax({
                        type: 'GET',
                        url: url,
                        contentType: 'application/json',
                        success: function(res){
                            if(res.data){
                                var json_data = JSON.parse(res.data);
                                if(json_data['device_name']===undefined || json_data['status']===undefined)
                                    return;

                                var new_row = document.getElementById(json_data['device_name']);
                                console.log(new_row);
                                if(new_row===null){
                                    new_row = document.createElement("tr");
                                    var device_name = document.createElement("td");
                                    device_name.id = "name";
                                    var status = document.createElement("td");
                                    status.id="status";
                                    device_name.innerHTML = json_data['device_name'];
                                    status.innerHTML = json_data['status'];
                                    new_row.append(device_name);
                                    new_row.append(status);
                                    new_row.id = json_data['device_name'];
                                    document.getElementById("devices-config").appendChild(new_row);
                                } else {
                                    var device_name = new_row.querySelector("#name");
                                    var status = new_row.querySelector("#status");
                                    device_name.textContent = json_data['device_name'];
                                    status.textContent = json_data['status'];
                                }
                            }  
                        },
                        error: function(xhr, status, error) {
                            console.error('Error:', error);
                        }
                    });
                });
            }


        },
        error: function(xhr, status, error) {
            console.error('Error sending POST request:', error);
        }
    });
}

setInterval(fetchData, 10000);

fetchData();
