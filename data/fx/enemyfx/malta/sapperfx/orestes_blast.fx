#############################################################
## Orestes_Blast.fx
#############################################################

FxInfo

Lifespan 150

########################################################

Condition
	On	Time
	Time	0

	Event
		EName 	GunTip
		Type	local
		At	WepR
		BhvrOverride
			PositionOffset		2.5 0 .3
		End
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	origin
		Sound Orestesrifle2 90 90 .9
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	GunTip
		Part	:ElectricityArchSmall.part
		Part	:ElectricityArchSmall2.part
		Part	:ElectricityArchSmallWhite.part
		Part	:ElectricityArchSmall2White.part
		Part	:SoulTickler_GunTipAttackGlow.Part
		POther	Target
		Lifespan 70
	End

	Event
		Type	Local
		At	GunTip
		Part	:SoulTickler_Stream01.part
		Part	:SoulTickler_Stream02.part
		Part	:SoulTickler_Stream03.part
		Part	:SoulTickler_Stream04.part
		POther	Target
		Lifespan 60
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	Target
		Part	:ElectricityArchSmall.part
		Part	:ElectricityArchSmall2.part
		Part	:ElectricityArchSmallWhite.part
		Part	:ElectricityArchSmall2White.part
		Part	:SoulTickler_GunTipAttackGlow.Part
		Lifespan 70
	End

	Event
		Type	Local
		At	Target
		Part	:Soul_Tickler_TargetGlow.Part
		Lifespan 72
	End
End

#############################################################

End