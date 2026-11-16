#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Start 
		At	Chest
		Part1	:SlagDust1.part
		Part2	:SlagDust2.part
		Part	:SlagChunkHitFlash.part
		Part	:SlagDustRocks2.part
		Part4	:SlagGolemMeleeImpact.part
		#Bhvr	Behaviors/CameraShake02Subtle.bhvr
		Sound boulderhit 90 90 .99

	End



End


End
