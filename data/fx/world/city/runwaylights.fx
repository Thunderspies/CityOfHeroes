#########################################################
##	SStreetSteeam01
##
FxInfo
	

#######################################################################
###########################################################
Condition
	On 	cycle
	Time 	40

	Event
		Type	Local
		At	Root
		ChildFx  World/City/RunwayLightsCycle.fx
		#ChildFx  World/City/RedStreetLightPulsing.fx
		#Geom FX_ICEWEAPON
		lifeSpan	100
	End
End

End			