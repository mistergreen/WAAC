// JavaScript Document

function deleteDevice() {
	var id = localStorage.getItem("deviceId");
	if(id != 0) {
		if(confirm("Are you sure you want to delete this device?")) {
			nocache = "&nocache=" + Math.random() * 1000000;
			$.get("?deletedevice=" + id + nocache, function(data, status, response) {
				if(data != null) {
				//on complete save, go back to home
					if($(data).find("response").text() == "success") {
						location.href="index.htm";
					}
				}
				
			});
		}//confirm
	} else {
		location.href="index.htm";
	}//id
}
	
function cancel() {
	var id = localStorage.getItem("deviceId");
	if(id != 0) {

			$.get("?canceldevice=" + id, function(data) {
				if(data != null) {
				//on complete save, go back to home
					if($(data).find("response").text() == "success") {
						location.href="index.htm";
					}
				}
				
			});
	
	} else {
		location.href="index.htm";
	}//id
}

function switchState() {
				
	var id = localStorage.getItem("deviceId");
	if(id != 0) {
		nocache = "&nocache=" + Math.random() * 1000000;
		$.get("?switch=" + id + nocache, function(data, status, response) {
				//no need for a response	
		});
	}
}
