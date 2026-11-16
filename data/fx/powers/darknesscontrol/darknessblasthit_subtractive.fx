#############################################################
## DarknessBlastHit_Subtractive.fx
#############################################################

FxInfo
LifeSpan	20

#############################################################

Input
	InpName	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0



	Event
		ENAME   Tendrills
		Type	Local
		At	Hips

		Part	:DarkBlastHitTendril2_Subtractive.part
		Part1	:DarkBlastHitTendril2.part
		Part	:DarkBlastHitPuddle2_Subtractive.Part
		Part2	:DarkBlastHitPuddle2.part

	End

End

End


