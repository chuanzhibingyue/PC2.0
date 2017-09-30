/**
 * Created by waouyang on 16/5/12.
 */
(function(uc){
    var _this = null;
    uc.modules.component.MeetingDurationPicker = function(options){
        _this = this;

        this.options = options;
        this.nodeHtml = '<div class="meeting-duration-container"></div>';
        this.node = $(this.nodeHtml).appendTo(this.options.parentNode);

        this.dropdownList = new uc.modules.component.DropdownList(this.node, {
            width: 80,
            items: uc_resource.Meeting.MeetingDurationSelect,
            selectedByText: false,
            selectedValue: 60,
            unit: uc_resource.Meeting.Minutes,
            onItemSelected: function(selectedItem){
                _this.onItemSelected(selectedItem);
            }
        })
    };

    jQuery.extend(uc.modules.component.MeetingDurationPicker.prototype, {
        onItemSelected: function(selectedItem){
            if(this.options.onItemSelected){
                this.options.onItemSelected(selectedItem);
            }
        },
        setValue: function(value){
            this.dropdownList.setValue(value);
        },
        getValue: function(){
            return this.dropdownList.getValue();
        }
    })
})(uc);