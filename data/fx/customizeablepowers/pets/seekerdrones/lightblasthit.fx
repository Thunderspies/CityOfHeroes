#############################################################
## LightBlastHit.fx
#############################################################

FxInfo
LifeSpan	100

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	Hips
		Part1	:LightBlastHitFlash.part
		Part2	:LightBlastHitEmber.part
		Part3	:LightBlastStreaks.part
		Part4	:LightBlastRays.part
		Part5	:LightBlastHandRing.part
	End


End

#############################################################

End


