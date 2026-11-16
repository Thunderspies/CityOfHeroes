#############################################################
## Child_SteamJet1.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part	:Steam_Jet.part
		Part	:Steam_Plume.part
	End
End

#############################################################
Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Sound1
		Type	Local
		At	Origin
		Sound RobotSelfDestructSteam_01 60 60 .5

	End

	Event
		EName	Sound2
		Type	Local
		At	Origin
		Sound RobotSelfDestructSteam_02 60 60 .6
	End

	Event
		EName	Sound3
		Type	Local
		At	Origin
		Sound RobotSelfDestructSteam_01 60 60 .5

	End

	Event
		EName	Sound4
		Type	Local
		At	Origin
		Sound RobotSelfDestructSteam_02 60 60 .45
	End
End
##############################################################
End