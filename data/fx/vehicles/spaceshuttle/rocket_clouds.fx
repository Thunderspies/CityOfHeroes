#########################################################
##	Warburg Rocket - Launch
##

FxInfo

Condition

	On Cycle 
	Time 5
	Chance	.75

	Event
		EName 	Cloud
		Type	Start
		Bhvr	:CloudScaler.bhvr
		#Geom	CloudBall
		part	:Rocket_Cloud_Smoke.part
		Lifespan	150
	End

End

End