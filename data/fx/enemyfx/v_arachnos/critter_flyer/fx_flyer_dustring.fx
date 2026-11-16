#########################################################
##	template

FxInfo

Flags  InheritAlpha   InheritAnimScale
lifespan	3

########  Doors  #############################################

Condition
	On 	time
	Time 	0


	Event
		ename	droptest1
		Type	startpositonly
		At 	origin
#		geom	testing_target
		part	:FX_Flyer_dustring.Part
		part	:FX_Flyer_dustring_light.Part
		part	:FX_Flyer_dustring_flat.Part
	End



End




End

