#########################################################
##	jack
##
FxInfo

Input  
	InpName	Chest
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Chest
		Geom	CreyAntennaLight
	End

	Event
		EName 	Bubbles
		Type	Local 
		At	Prime
		Altpiv	1
		Part1	EnemyFx/SkyRaiders/WingLightBlue.part
		Part2	EnemyFx/SkyRaiders/WingLightCenterBlue.part
	End

End

End			