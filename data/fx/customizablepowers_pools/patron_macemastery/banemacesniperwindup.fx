#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

Input
	InpName	Origin
End

Input
	InpName	Target
End

Input
	Inpname	Head
End


#########################################################

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At 	origin
		Sound cock2 60 60 .46
	End
End


Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At 	origin
		Sound rumble7 60 60 .3
	End
End


Condition
	On 	Time
	Time 	140

	Event
		Type	Local
		At 	origin
		Sound maceramp 60 60 .45
	End
End





Condition
	On 	Time
	Time 	0

	Event
		EName	Point
		Type	local
		At	wepR
		Geom	heavyMace
	End

	Event
		EName	Prime
		Type	Local
		At	Point
		Altpiv	2

		Magnet	Prime
		LookAt	Prime


		Part	:EnergyHeadBuildUP.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part
		Part	:BotTrailBuildUP.part
		Part	:BotTrailSpecksBuildUP.part
		Part	:BotBubblesBuildUP.part

		Sound BaneMaceCharge_loop 80 80 .7

	End


End

End