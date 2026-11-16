#############################################################
## MoonBeamBlastHit.fx
#############################################################

FxInfo
LifeSpan	30

########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME   Tendrills
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\DarkBlast\WhiteBlast.part
	End

	Event
		ENAME   Tendrills2
		Type	Local
		At	Hips
		Part1	CustomizeablePowers\DarkBlast\MoonBeamLightRaysHit.part
		Part2	CustomizeablePowers\DarkBlast\DarkBlastHitPuddle2.part
		Lifespan 	21
	End

End

#########################################################

End


