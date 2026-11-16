#########################################################
##	RadiationAura
FxInfo

########################################################

Condition
	On	Cycle
	Time	4

	Event
		ENAME	Glow
		Type	SetLight	
		Bhvr	:NukeGlow.bhvr

		Lifespan 7
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		
		part2	POWERS\RadiationControl\RadiationBubbleBlastDotsHitx.part
		Part3	POWERS\RadiationControl\RadiationBubblesHitx.part
		Part4	POWERS\RadiationControl\RadiationBubblesSlowx.part
		part5	POWERS\RadiationControl\RadiationAuraYellowHit2x.part
		PMagnet	Hips
		
		#Sound 	plasmafire_loop 86 22 .88
		
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
	
		part5	POWERS\RadiationControl\RadiationAuraYellowHit3x.part
		PMagnet	Hips
		
	End

End


End				