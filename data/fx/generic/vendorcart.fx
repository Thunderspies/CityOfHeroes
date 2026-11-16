
FxInfo

#####  Cement Bucket   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	root					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
			
		bhvr	behaviors\FX_VendorCart.bhvr
		Geom	Vendor_Cart				#call to geometry fx
	End

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	root					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		
		bhvr	behaviors\FX_VendorCart.bhvr
		Geom	Vendor_Umbrella01
		Geom	Vendor_Umbrella02
		Geom	Vendor_Nothing
	End

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	root					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		
		bhvr	behaviors\FX_VendorCart.bhvr
		Geom	Vendor_Register				#call to geometry fx
	End
End

End		