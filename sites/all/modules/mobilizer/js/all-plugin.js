/**

**** Upload File ------
* Bootstrap.js by @mdo and @fat, extended by @ArnoldDaniels.
* plugins: bootstrap-fileupload.js
* Copyright 2012 Twitter, Inc.
* http://www.apache.org/licenses/LICENSE-2.0.txt
*/
!function(a){
    var b=function(b,c){
        this.$element=a(b),this.type=this.$element.data("uploadtype")||(this.$element.find(".thumbnail").length>0?"image":"file"),this.$input=this.$element.find(":file");
        if(this.$input.length===0)return;
        this.name=this.$input.attr("name")||c.name,this.$hidden=this.$element.find('input[type=hidden][name="'+this.name+'"]'),this.$hidden.length===0&&(this.$hidden=a('<input type="hidden" />'),this.$element.prepend(this.$hidden)),this.$preview=this.$element.find(".fileupload-preview");
        var d=this.$preview.css("height");
        this.$preview.css("display")!="inline"&&d!="0px"&&d!="none"&&this.$preview.css("line-height",d),this.original={
            exists:this.$element.hasClass("fileupload-exists"),
            preview:this.$preview.html(),
            hiddenVal:this.$hidden.val()
            },this.$remove=this.$element.find('[data-dismiss="fileupload"]'),this.$element.find('[data-trigger="fileupload"]').on("click.fileupload",a.proxy(this.trigger,this)),this.listen()
        };
        
    b.prototype={
        listen:function(){
            this.$input.on("change.fileupload",a.proxy(this.change,this)),a(this.$input[0].form).on("reset.fileupload",a.proxy(this.reset,this)),this.$remove&&this.$remove.on("click.fileupload",a.proxy(this.clear,this))
            },
        change:function(a,b){
            var c=a.target.files!==undefined?a.target.files[0]:a.target.value?{
                name:a.target.value.replace(/^.+\\/,"")
                }:null;
            if(b==="clear")return;
            if(!c){
                this.clear();
                return
            }
            this.$hidden.val(""),this.$hidden.attr("name",""),this.$input.attr("name",this.name);
            if(this.type==="image"&&this.$preview.length>0&&(typeof c.type!="undefined"?c.type.match("image.*"):c.name.match("\\.(gif|png|jpe?g)$"))&&typeof FileReader!="undefined"){
                var d=new FileReader,e=this.$preview,f=this.$element;
                d.onload=function(a){
                    e.html('<img src="'+a.target.result+'" '+(e.css("max-height")!="none"?'style="max-height: '+e.css("max-height")+';"':"")+" />"),f.addClass("fileupload-exists").removeClass("fileupload-new")
                    },d.readAsDataURL(c)
                }else this.$preview.text(c.name),this.$element.addClass("fileupload-exists").removeClass("fileupload-new")
                },
        clear:function(b){
            this.$hidden.val(""),this.$hidden.attr("name",this.name),this.$input.attr("name","");
            if(a.browser.msie){
                var c=this.$input.clone(!0);
                this.$input.after(c),this.$input.remove(),this.$input=c
                }else this.$input.val("");
            this.$preview.html(""),this.$element.addClass("fileupload-new").removeClass("fileupload-exists"),b&&(this.$input.trigger("change",["clear"]),b.preventDefault())
            },
        reset:function(a){
            this.clear(),this.$hidden.val(this.original.hiddenVal),this.$preview.html(this.original.preview),this.original.exists?this.$element.addClass("fileupload-exists").removeClass("fileupload-new"):this.$element.addClass("fileupload-new").removeClass("fileupload-exists")
            },
        trigger:function(a){
            this.$input.trigger("click"),a.preventDefault()
            }
        },a.fn.fileupload=function(c){
    return this.each(function(){
        var d=a(this),e=d.data("fileupload");
        e||d.data("fileupload",e=new b(this,c)),typeof c=="string"&&e[c]()
        })
    },a.fn.fileupload.Constructor=b,a(function(){
//    a("body").on("click.fileupload.data-api",'[data-provides="fileupload"]',function(b){
//        var c=a(this);
//        if(c.data("fileupload"))return;
//        c.fileupload(c.data());
//        var d=a(b.target).is("[data-dismiss=fileupload],[data-trigger=fileupload]")?a(b.target):a(b.target).parents("[data-dismiss=fileupload],[data-trigger=fileupload]").first();
//        d.length>0&&(d.trigger("click.fileupload"),b.preventDefault())
//        })
    })
}(window.jQuery)

/**

/**

****
Select Menu
*/

!function(b){
    var a=function(f,c,g){
        if(g){
            g.stopPropagation();
            g.preventDefault()
            }
            this.$element=b(f);
        this.$newElement=null;
        var d=null;
        this.options=b.extend({},b.fn.selectpicker.defaults,this.$element.data(),typeof c=="object"&&c);
        this.style=this.options.style;
        this.size=this.options.size;
        this.init()
        };
        
    a.prototype={
        constructor:a,
        init:function(m){
            this.$element.hide();
            var d=this.$element.attr("class")!==undefined?this.$element.attr("class").split(/\s+/):"";
            var p=this.getTemplate();
            var c=this.$element.attr("id");
            p=this.createLi(p);
            this.$element.after(p);
            this.$newElement=this.$element.next(".bootstrap-select");
            var o=this.$newElement;
            var f=this.$newElement.find(".dropdown-menu");
            var l=this.$newElement.find(".dropdown-menu ul li > a");
            var n=parseInt(l.css("line-height"))+l.outerHeight();
            var h=this.$newElement.offset().top;
            var r=0;
            var g=0;
            var k=this.$newElement.outerHeight();
            button=this.$newElement.find("> button");
            if(c!==undefined){
                button.attr("id",c);
                b('label[for="'+c+'"]').click(function(){
                    o.find("button#"+c).focus()
                    })
                }
                for(var j=0;j<d.length;j++){
                if(d[j]!="selectpicker"){
                    this.$newElement.addClass(d[j])
                    }
                }
            button.addClass(this.style);
        this.checkDisabled();
        this.clickListener();
        this.$element.find("optgroup").each(function(){
            if(b(this).attr("label")){
                f.find(".opt"+b(this).index()).eq(0).before("<dt>"+b(this).attr("label")+"</dt>")
                }
                f.find(".opt"+b(this).index()).eq(0).parent().prev().addClass("optgroup-div")
            });
        if(this.size=="auto"){
            function q(){
                var e=h-b(window).scrollTop();
                var t=window.innerHeight;
                var i=parseInt(f.css("padding-top"))+parseInt(f.css("padding-bottom"))+parseInt(f.css("border-top-width"))+parseInt(f.css("border-bottom-width"))+parseInt(f.css("margin-top"))+parseInt(f.css("margin-bottom"))+2;
                var s=t-e-k-i;
                if(!o.hasClass("dropup")){
                    r=Math.floor(s/n)
                    }else{
                    r=Math.floor((e-i)/n)
                    }
                    if(r<4){
                    r=3
                    }
                    g=n*r;
                if(f.find("ul li").length+f.find("dt").length>r){
                    f.find("ul").css({
                        "max-height":g+"px",
                        "overflow-y":"scroll"
                    })
                    }else{
                    f.find("ul").css({
                        "max-height":"none",
                        "overflow-y":"auto"
                    })
                    }
                }
            q();
        b(window).resize(q);
        b(window).scroll(q)
        }else{
        if(this.size&&this.size!="auto"&&f.find("ul li").length>this.size){
            g=n*this.size;
            if(this.size==1){
                g=g+8
                }
                f.find("ul").css({
                "max-height":g+"px",
                "overflow-y":"scroll"
            })
            }
        }
    this.$newElement.find("ul").bind("DOMNodeInserted",b.proxy(this.clickListener,this))
    },
getTemplate:function(){
    var c="<div class='btn-group bootstrap-select'><button class='btn dropdown-toggle clearfix' data-toggle='dropdown'><span class='filter-option pull-left'>__SELECTED_OPTION</span>&nbsp;<span class='caret'></span></button><div class='dropdown-menu' role='menu'><ul>__ADD_LI</ul></div></div>";
    return c
    },
createLi:function(f){
    var e=[];
    var k=[];
    var c="";
    var g=null;
    var j=this;
    var h=this.$element[0].selectedIndex?this.$element[0].selectedIndex:0;
    this.$element.find("option").each(function(){
        e.push(b(this).text())
        });
    this.$element.find("option").each(function(){
        if(b(this).parent().is("optgroup")){
            g=String(b(this).parent().index());
            var l=b(this).parent();
            for(var m=0;m<l.length;m++){
                k.push('<a class="opt'+g[m]+'" tabindex="-1" href="#">'+b(this).text()+"</a>")
                }
            }else{
        k.push('<a tabindex="-1" href="#">'+b(this).text()+"</a>")
        }
    });
if(e.length>0){
    f=f.replace("__SELECTED_OPTION",e[h]);
    for(var d=0;d<e.length;d++){
        c+="<li rel="+d+">"+k[d]+"</li>"
        }
    }
    this.$element.find("option").eq(h).prop("selected",true);
f=f.replace("__ADD_LI",c);
return f
},
checkDisabled:function(){
    if(this.$element.is(":disabled")){
        button.addClass("disabled");
        button.click(function(c){
            c.preventDefault()
            })
        }
    },
clickListener:function(){
    var c=this;
    b("body").on("touchstart.dropdown",".dropdown-menu",function(d){
        d.stopPropagation()
        });
    b(".dropdown-menu").find("li dt").on("click",function(d){
        d.stopPropagation()
        });
    this.$newElement.find("li a").on("click",function(i){
        i.preventDefault();
        var g=b(this).parent().index();
        var h=b(this).parent(),d=h.attr("rel"),f=h.parents(".bootstrap-select");
        if(c.$element.not(":disabled")){
            f.prev("select").find("option").eq(g).prop("selected",true);
            f.find(".filter-option").html(h.text());
            f.find("button").focus();
            f.prev("select").trigger("change")
            }
        });
this.$element.on("change",function(f){
    var d=b(this).find("option:selected").text();
    b(this).next(".bootstrap-select").find(".filter-option").html(d)
    })
}
};

b.fn.selectpicker=function(c,d){
    return this.each(function(){
        var g=b(this),f=g.data("selectpicker"),e=typeof c=="object"&&c;
        if(!f){
            g.data("selectpicker",(f=new a(this,e,d)))
            }
            if(typeof c=="string"){
            f[c]()
            }
        })
};

b.fn.selectpicker.defaults={
    style:null,
    size:"auto"
}
}(window.jQuery);





/* =========================================================
	Date-Picker Plugin

 */
 
!function( $ ) {
	
    // Picker object
	
    var Datepicker = function(element, options){
        this.element = $(element);
        this.format = DPGlobal.parseFormat(options.format||this.element.data('date-format')||'mm/dd/yyyy');
        this.picker = $(DPGlobal.template)
        .appendTo('body')
        .on({
            click: $.proxy(this.click, this),
            mousedown: $.proxy(this.mousedown, this)
        });
        this.isInput = this.element.is('input');
        this.component = this.element.is('.date') ? this.element.find('.add-on') : false;
		
        if (this.isInput) {
            this.element.on({
                focus: $.proxy(this.show, this),
                blur: $.proxy(this.hide, this),
                keyup: $.proxy(this.update, this)
            });
        } else {
            if (this.component){
                this.component.on('click', $.proxy(this.show, this));
            } else {
                this.element.on('click', $.proxy(this.show, this));
            }
        }
        this.minViewMode = options.minViewMode||this.element.data('date-minviewmode')||0;
        if (typeof this.minViewMode === 'string') {
            switch (this.minViewMode) {
                case 'months':
                    this.minViewMode = 1;
                    break;
                case 'years':
                    this.minViewMode = 2;
                    break;
                default:
                    this.minViewMode = 0;
                    break;
            }
        }
        this.viewMode = options.viewMode||this.element.data('date-viewmode')||0;
        if (typeof this.viewMode === 'string') {
            switch (this.viewMode) {
                case 'months':
                    this.viewMode = 1;
                    break;
                case 'years':
                    this.viewMode = 2;
                    break;
                default:
                    this.viewMode = 0;
                    break;
            }
        }
        this.startViewMode = this.viewMode;
        this.weekStart = options.weekStart||this.element.data('date-weekstart')||0;
        this.weekEnd = this.weekStart === 0 ? 6 : this.weekStart - 1;
        this.fillDow();
        this.fillMonths();
        this.update();
        this.showMode();
    };
	
    Datepicker.prototype = {
        constructor: Datepicker,
		
        show: function(e) {
            this.picker.show();
            this.height = this.component ? this.component.outerHeight() : this.element.outerHeight();
            this.place();
            $(window).on('resize', $.proxy(this.place, this));
            if (e ) {
                e.stopPropagation();
                e.preventDefault();
            }
            if (!this.isInput) {
                $(document).on('mousedown', $.proxy(this.hide, this));
            }
            this.element.trigger({
                type: 'show',
                date: this.date
            });
        },
	
        hide: function(){
            this.picker.hide();
            $(window).off('resize', this.place);
            this.viewMode = this.startViewMode;
            this.showMode();
            if (!this.isInput) {
                $(document).off('mousedown', this.hide);
            }
            this.set();
            this.element.trigger({
                type: 'hide',
                date: this.date
            });
        },
	
        set: function() {
            var formated = DPGlobal.formatDate(this.date, this.format);
            if (!this.isInput) {
                if (this.component){
                    this.element.find('input').prop('value', formated);
                }
                this.element.data('date', formated);
            } else {
                this.element.prop('value', formated);
            }
        },
	
        setValue: function(newDate) {
            if (typeof newDate === 'string') {
                this.date = DPGlobal.parseDate(newDate, this.format);
            } else {
                this.date = new Date(newDate);
            }
            this.set();
            this.viewDate = new Date(this.date.getFullYear(), this.date.getMonth(), 1, 0, 0, 0, 0);
            this.fill();
        },
	
        place: function(){
            var offset = this.component ? this.component.offset() : this.element.offset();
            this.picker.css({
                top: offset.top + this.height,
                left: offset.left
            });
        },
	
        update: function(newDate){
            this.date = DPGlobal.parseDate(
                typeof newDate === 'string' ? newDate : (this.isInput ? this.element.prop('value') : this.element.data('date')),
                this.format
                );
            this.viewDate = new Date(this.date.getFullYear(), this.date.getMonth(), 1, 0, 0, 0, 0);
            this.fill();
        },
	
        fillDow: function(){
            var dowCnt = this.weekStart;
            var html = '<tr>';
            while (dowCnt < this.weekStart + 7) {
                html += '<th class="dow">'+DPGlobal.dates.daysMin[(dowCnt++)%7]+'</th>';
            }
            html += '</tr>';
            this.picker.find('.datepicker-days thead').append(html);
        },
	
        fillMonths: function(){
            var html = '';
            var i = 0
            while (i < 12) {
                html += '<span class="month">'+DPGlobal.dates.monthsShort[i++]+'</span>';
            }
            this.picker.find('.datepicker-months td').append(html);
        },
	
        fill: function() {
            var d = new Date(this.viewDate),
            year = d.getFullYear(),
            month = d.getMonth(),
            currentDate = this.date.valueOf();
            this.picker.find('.datepicker-days th:eq(1)')
            .text(DPGlobal.dates.months[month]+' '+year);
            var prevMonth = new Date(year, month-1, 28,0,0,0,0),
            day = DPGlobal.getDaysInMonth(prevMonth.getFullYear(), prevMonth.getMonth());
            prevMonth.setDate(day);
            prevMonth.setDate(day - (prevMonth.getDay() - this.weekStart + 7)%7);
            var nextMonth = new Date(prevMonth);
            nextMonth.setDate(nextMonth.getDate() + 42);
            nextMonth = nextMonth.valueOf();
            html = [];
            var clsName;
            while(prevMonth.valueOf() < nextMonth) {
                if (prevMonth.getDay() === this.weekStart) {
                    html.push('<tr>');
                }
                clsName = '';
                if (prevMonth.getMonth() < month) {
                    clsName += ' old';
                } else if (prevMonth.getMonth() > month) {
                    clsName += ' new';
                }
                if (prevMonth.valueOf() === currentDate) {
                    clsName += ' active';
                }
                html.push('<td class="day'+clsName+'">'+prevMonth.getDate() + '</td>');
                if (prevMonth.getDay() === this.weekEnd) {
                    html.push('</tr>');
                }
                prevMonth.setDate(prevMonth.getDate()+1);
            }
            this.picker.find('.datepicker-days tbody').empty().append(html.join(''));
            var currentYear = this.date.getFullYear();
			
            var months = this.picker.find('.datepicker-months')
            .find('th:eq(1)')
            .text(year)
            .end()
            .find('span').removeClass('active');
            if (currentYear === year) {
                months.eq(this.date.getMonth()).addClass('active');
            }
			
            html = '';
            year = parseInt(year/10, 10) * 10;
            var yearCont = this.picker.find('.datepicker-years')
            .find('th:eq(1)')
            .text(year + '-' + (year + 9))
            .end()
            .find('td');
            year -= 1;
            for (var i = -1; i < 11; i++) {
                html += '<span class="year'+(i === -1 || i === 10 ? ' old' : '')+(currentYear === year ? ' active' : '')+'">'+year+'</span>';
                year += 1;
            }
            yearCont.html(html);
        },
		
        click: function(e) {
            e.stopPropagation();
            e.preventDefault();
            var target = $(e.target).closest('span, td, th');
            if (target.length === 1) {
                switch(target[0].nodeName.toLowerCase()) {
                    case 'th':
                        switch(target[0].className) {
                            case 'switch':
                                this.showMode(1);
                                break;
                            case 'prev':
                            case 'next':
                                this.viewDate['set'+DPGlobal.modes[this.viewMode].navFnc].call(
                                    this.viewDate,
                                    this.viewDate['get'+DPGlobal.modes[this.viewMode].navFnc].call(this.viewDate) + 
                                    DPGlobal.modes[this.viewMode].navStep * (target[0].className === 'prev' ? -1 : 1)
                                    );
                                this.fill();
                                this.set();
                                break;
                        }
                        break;
                    case 'span':
                        if (target.is('.month')) {
                            var month = target.parent().find('span').index(target);
                            this.viewDate.setMonth(month);
                        } else {
                            var year = parseInt(target.text(), 10)||0;
                            this.viewDate.setFullYear(year);
                        }
                        if (this.viewMode !== 0) {
                            this.date = new Date(this.viewDate);
                            this.element.trigger({
                                type: 'changeDate',
                                date: this.date,
                                viewMode: DPGlobal.modes[this.viewMode].clsName
                            });
                        }
                        this.showMode(-1);
                        this.fill();
                        this.set();
                        break;
                    case 'td':
                        if (target.is('.day')){
                            var day = parseInt(target.text(), 10)||1;
                            var month = this.viewDate.getMonth();
                            if (target.is('.old')) {
                                month -= 1;
                            } else if (target.is('.new')) {
                                month += 1;
                            }
                            var year = this.viewDate.getFullYear();
                            this.date = new Date(year, month, day,0,0,0,0);
                            this.viewDate = new Date(year, month, Math.min(28, day),0,0,0,0);
                            this.fill();
                            this.set();
                            this.element.trigger({
                                type: 'changeDate',
                                date: this.date,
                                viewMode: DPGlobal.modes[this.viewMode].clsName
                            });
                        }
                        break;
                }
            }
        },
	
        mousedown: function(e){
            e.stopPropagation();
            e.preventDefault();
        },
		
        showMode: function(dir) {
            if (dir) {
                this.viewMode = Math.max(this.minViewMode, Math.min(2, this.viewMode + dir));
            }
            this.picker.find('>div').hide().filter('.datepicker-'+DPGlobal.modes[this.viewMode].clsName).show();
        }
    };
	
    $.fn.datepicker = function ( option, val ) {
        return this.each(function () {
            var $this = $(this),
            data = $this.data('datepicker'),
            options = typeof option === 'object' && option;
            if (!data) {
                $this.data('datepicker', (data = new Datepicker(this, $.extend({}, $.fn.datepicker.defaults,options))));
            }
            if (typeof option === 'string') data[option](val);
        });
    };

    $.fn.datepicker.defaults = {
    };
    $.fn.datepicker.Constructor = Datepicker;
	
    var DPGlobal = {
        modes: [
        {
            clsName: 'days',
            navFnc: 'Month',
            navStep: 1
        },
        {
            clsName: 'months',
            navFnc: 'FullYear',
            navStep: 1
        },
        {
            clsName: 'years',
            navFnc: 'FullYear',
            navStep: 10
        }],
        dates:{
            days: ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"],
            daysShort: ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"],
            daysMin: ["Su", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"],
            months: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"],
            monthsShort: ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
        },
        isLeapYear: function (year) {
            return (((year % 4 === 0) && (year % 100 !== 0)) || (year % 400 === 0))
        },
        getDaysInMonth: function (year, month) {
            return [31, (DPGlobal.isLeapYear(year) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31][month]
        },
        parseFormat: function(format){
            var separator = format.match(/[.\/\-\s].*?/),
            parts = format.split(/\W+/);
            if (!separator || !parts || parts.length === 0){
                throw new Error("Invalid date format.");
            }
            return {
                separator: separator, 
                parts: parts
            };
        },
        parseDate: function(date, format) {
            var parts = date.split(format.separator),
            date = new Date(),
            val;
            date.setHours(0);
            date.setMinutes(0);
            date.setSeconds(0);
            date.setMilliseconds(0);
            if (parts.length === format.parts.length) {
                for (var i=0, cnt = format.parts.length; i < cnt; i++) {
                    val = parseInt(parts[i], 10)||1;
                    switch(format.parts[i]) {
                        case 'dd':
                        case 'd':
                            date.setDate(val);
                            break;
                        case 'mm':
                        case 'm':
                            date.setMonth(val - 1);
                            break;
                        case 'yy':
                            date.setFullYear(2000 + val);
                            break;
                        case 'yyyy':
                            date.setFullYear(val);
                            break;
                    }
                }
            }
            return date;
        },
        formatDate: function(date, format){
            var val = {
                d: date.getDate(),
                m: date.getMonth() + 1,
                yy: date.getFullYear().toString().substring(2),
                yyyy: date.getFullYear()
            };
            val.dd = (val.d < 10 ? '0' : '') + val.d;
            val.mm = (val.m < 10 ? '0' : '') + val.m;
            var date = [];
            for (var i=0, cnt = format.parts.length; i < cnt; i++) {
                date.push(val[format.parts[i]]);
            }
            return date.join(format.separator);
        },
        headTemplate: '<thead>'+
        '<tr>'+
        '<th class="prev">&lsaquo;</th>'+
        '<th colspan="5" class="switch"></th>'+
        '<th class="next">&rsaquo;</th>'+
        '</tr>'+
        '</thead>',
        contTemplate: '<tbody><tr><td colspan="7"></td></tr></tbody>'
    };
    DPGlobal.template = '<div class="datepicker dropdown-menu">'+
    '<div class="datepicker-days">'+
    '<table class=" table-condensed">'+
    DPGlobal.headTemplate+
    '<tbody></tbody>'+
    '</table>'+
    '</div>'+
    '<div class="datepicker-months">'+
    '<table class="table-condensed">'+
    DPGlobal.headTemplate+
    DPGlobal.contTemplate+
    '</table>'+
    '</div>'+
    '<div class="datepicker-years">'+
    '<table class="table-condensed">'+
    DPGlobal.headTemplate+
    DPGlobal.contTemplate+
    '</table>'+
    '</div>'+
    '</div>';

}( window.jQuery )
