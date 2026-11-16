#############################################################
## DarknessBackstabHit.fx
#############################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5
LifeSpan 20

#############################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Part	CustomizeablePowers\DarkMelee\Souls.part
		Part	CustomizeablePowers\DarkMelee\Souls2.part
		Part	CustomizeablePowers\DarkMelee\Souls3.part
		Part	CustomizeablePowers\DarkMelee\Souls4.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastHitTendril3_Subtractive.part
		Part1	CustomizeablePowers\DarkMelee\DarkBlastHitTendril2.part
		Part	CustomizeablePowers\DarkMelee\DarkBlastPuddle2_Subtractive.Part
		Part2	CustomizeablePowers\DarkMelee\DarkBlastHitPuddle2.part
		Sound Hit3 100.0 100.0 .8
	End
End

#############################################################

End


