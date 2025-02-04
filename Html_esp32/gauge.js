// © 2noodles llc

	function gauge(inClass) {

		$('.'+inClass).each(function(i, obj){
			inID = $(this).attr("id");
			//add attribute
			var cwidth = $('#'+inID).data("size");
			var cheight = $('#'+inID).data("size");
			var val = $('#'+inID).data("value");

			$("<span/>", {
				class: "info",
				id: "container_"+inID,
				style: "margin: 0 "+ cwidth/4+"px; padding-top: "+ cheight/3 +"px; position:absolute; text-align: center; width: "+cwidth/2 + "px;"
			}).appendTo( $('#'+inID) );

			$("<b/>", {
				class: "value",
				style: "font-family: Arial,sans-serif; font-weight: normal; font-size: "+cheight/4.5+"px; color: "+ $('#'+inID).data("barColor") +";",
				text: val
			}).appendTo( $('#container_'+inID) );

			$("<u/>", {
				class: "unit",
				style: "text-decoration: none; font-family: Arial,sans-serif; font-size: "+cheight/8+"px; color: "+ $('#'+inID).data("barColor") +";",
				text: $('#'+inID).data("unit")
			}).appendTo( $('#container_'+inID) );

			$("<p/>", {
				class: "title",
				style: "margin-top: "+ cheight/10+"px; text-decoration: none; font-family: Arial,sans-serif; font-size: "+cheight/14+"px; color: "+ $('#'+inID).data("barColor") +";",
				text: $('#'+inID).data("name")
			}).appendTo( $('#container_'+inID) );

			$('#'+inID).append('<canvas id="canvas_'+inID+'" width="'+cwidth+'" heigh="'+cheight+'"></canvas>')
			drawGauge(inID, val);
			
			
		})	
		
	}
	
	function drawGauge(inID, inVal) {

		var c =  $('#'+inID +' canvas').get(0);
		var cwidth = $('#'+inID).data("size");
		var cheight = $('#'+inID).data("size");
		
		var linewidth = $('#'+inID).data("lineWidth");
		var val = inVal;
		var min = $('#'+inID).data("min") !== undefined ? $('#'+inID).data("min") : 0;
		var max = $('#'+inID).data("max") !== undefined ? $('#'+inID).data("max") : 100;
		
		var percent = val/(max-min);
			
		var ctx = c.getContext("2d");
		ctx.clearRect(0, 0, cwidth, cheight);
		//back
		ctx.lineWidth= linewidth;
		ctx.beginPath();
		ctx.arc(cwidth/2, cheight/2, cwidth/2-linewidth, 0.75*Math.PI, 0.25*Math.PI);
		ctx.strokeStyle = $('#'+inID).data("backbarColor");
		ctx.lineCap = 'round';
		ctx.stroke();
		
		// front bar
		ctx.lineWidth= linewidth+0.5;
		ctx.beginPath();
		var endarc = (270*percent)*Math.PI/180 + 135*Math.PI/180;
		ctx.arc(cwidth/2, cheight/2,cwidth/2-linewidth, 0.75*Math.PI, endarc);
		ctx.strokeStyle = $('#'+inID).data("barColor");
		ctx.lineCap = 'round';
		ctx.stroke();
		
		
		
	}
	
	function update(inID, inVal) {
		$('#container_'+inID+' b').text(inVal);
		//console.log("meter: "+inVal);
		drawGauge(inID, inVal);
		
	}