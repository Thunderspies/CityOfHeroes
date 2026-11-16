#########################################################
##	Fireweapon
########################################################
FxInfo


Flags    InheritAlpha

########################################################

Condition	

	Event
		EName 	light
		Type	Local 
		At		ROOT
		Geom	StealthCameraCone 
		bhvr	:weirdconeSpinOpposite.bhvr
	 
	End

	Event
		EName 	light
		Type	Local 
		At	ROOT
		Geom	StealthCameraCone 
		bhvr	:weirdcone.bhvr
	End



End

##################################

