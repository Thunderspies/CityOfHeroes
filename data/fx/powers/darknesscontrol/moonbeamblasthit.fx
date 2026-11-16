#############################################################
## MoonBeamBlastHit.fx
#############################################################

FxInfo
LifeSpan	30

#########################################################

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
		Part1	:WhiteBlast.part
		#Part1	:MoonBeamLightRaysHit.part

	End

	Event
		ENAME   Tendrills2
		Type	Local
		At	Hips
		Part1	:MoonBeamLightRaysHit.part
		Part2	:DarkBlastHitPuddle2.part

	End

End

Condition
	On 	Time
	Time 	21
	Event
		ENAME   Tendrills2
		Type	destroy

	End

End

End


