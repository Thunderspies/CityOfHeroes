#########################################################
##	Warburg Rocket - Launch
##

FxInfo

##############################################################
##Smoke
#############################################

Condition

	On Cycle 
	Time 1

	Event
		EName 	Cloud
		Type	local
		
		BhvrOverride
			positionOffset	0 -52 0
		End

		Bhvr	:IgnitionRotation.bhvr
		part	:Rocket_ignition_Smoke.part
		Lifespan	10
	End

End

End