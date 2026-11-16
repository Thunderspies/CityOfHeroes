#########################################################
##	RadiationAura
FxInfo

#LifeSpan	40

Input  
	InpName	Hips
End

########################################################

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	hips
		Sound goo_loop 60 60 .27
		bhvr	CustomizeablePowers\RadiationEmission\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 340
	End
End



Condition
	On	Time
	Time	7

	Event
		
		Type	Posit
		At	Hips
		#part1	:RadiationElectronsHit.part
		part2	CustomizeablePowers\RadiationEmission\RadiationBubbleBlastDotsHit.part
		#Part3	:RadiationBubblesHit.part
		Part4	CustomizeablePowers\RadiationEmission\RadiationBubblesSlow.part
		#part5	:RadiationAuraYellowHit2.part
		#Sound	goo_loop 60 30 .3
		PMagnet	Hips
		
	End

	Event
		
		Type	Posit
		At	Hips
		
		#part4	:RadiationBlueCloud3.part
		#part5	:RadiationAuraYellowHit3.part
		Sound goohiss 60 60 .22
		PMagnet	Hips
		
	End




	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		
		part2	CustomizeablePowers\RadiationEmission\RadiationBubbleBlastDotsHitx.part
		#Part3	:RadiationBubblesHitx.part
		Part4	CustomizeablePowers\RadiationEmission\RadiationBubblesSlowx.part
		#part5	:RadiationAuraYellowHit2x.part
		PMagnet	Hips
		
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		
		part4	CustomizeablePowers\RadiationEmission\RadiationBlueCloud.part
		#part5	:RadiationAuraYellowHit3x.part
		PMagnet	Hips
		
	End

End


End				