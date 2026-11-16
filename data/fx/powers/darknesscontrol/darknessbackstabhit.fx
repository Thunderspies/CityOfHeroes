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
		Part	:Souls.part
		Part	:Souls2.part
		Part	:Souls3.part
		Part	:Souls4.part
		Part	:DarkBlastHitTendril3_Subtractive.part
		Part1	:DarkBlastHitTendril2.part
		Part	:DarkBlastPuddle2_Subtractive.Part
		Part2	:DarkBlastHitPuddle2.part
		Sound Hit3 100.0 100.0 .8
	End
End

#############################################################

End


