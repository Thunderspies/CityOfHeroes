#########################################################
##	template

FxInfo
LifeSpan		600

#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		#Sound	Glow3_loop 80 30 .26
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Prime
		Type	start
		At	T_root
		BHVR	:Projectile.Bhvr
		Geom	NanoMachine
		Part	:NanomiteLightGlows.part

		Part	:NanomiteLightBurst.part
		Part	:NanomiteLightRays.part
		
		Part	:NanomiteLightRings.part
		Part	:NanomiteLightRings2.part
		
		Magnet	Root
		Lookat	Root
	End

End

Condition
	On	cycle
	Time	9
	Chance	.8

	Event	
		Ename	Light
		Type	local
		At	Prime
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:NanomiteLight.part
		Part	:NanomiteLightstar.part
		LifeSpan	3
	End
End
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Prime1
		Type	start
		At	T_root
		BHVR	:Projectile1.Bhvr
		Geom	NanoMachine
		Part	:NanomiteLightGlows.part

		Part	:NanomiteLightBurst.part
		Part	:NanomiteLightRays.part

		Part	:NanomiteLightRings.part
		Part	:NanomiteLightRings2.part
		
		Magnet	Root
		Lookat	Root
	End

End

Condition
	On	cycle
	Time	9
	Chance	.8

	Event	
		Ename	Light1
		Type	local
		At	Prime1
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:NanomiteLight.part
		Part	:NanomiteLightstar.part
		LifeSpan	3
	End
End
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Prime2
		Type	start
		At	T_root
		BHVR	:Projectile2.Bhvr
		Geom	NanoMachine
		Part	:NanomiteLightGlows.part

		Part	:NanomiteLightBurst.part
		Part	:NanomiteLightRays.part

		Part	:NanomiteLightRings.part
		Part	:NanomiteLightRings2.part
		
		Magnet	Root
		Lookat	Root
	End

End

Condition
	On	cycle
	Time	9
	Chance	.8

	Event	
		Ename	Light2
		Type	local
		At	Prime2
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:NanomiteLight.part
		Part	:NanomiteLightstar.part
		LifeSpan	3
	End
End
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Prime3
		Type	start
		At	T_root
		BHVR	:Projectile3.Bhvr
		Geom	NanoMachine
		Part	:NanomiteLightGlows.part

		Part	:NanomiteLightBurst.part
		Part	:NanomiteLightRays.part

		Part	:NanomiteLightRings.part
		Part	:NanomiteLightRings2.part
		
		Magnet	Root
		Lookat	Root
	End

End

Condition
	On	cycle
	Time	9
	Chance	.8

	Event	
		Ename	Light3
		Type	local
		At	Prime3
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:NanomiteLight.part
		Part	:NanomiteLightstar.part
		LifeSpan	3
	End
End
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Prime4
		Type	start
		At	T_root
		BHVR	:Projectile4.Bhvr
		Geom	NanoMachine
		Part	:NanomiteLightGlows.part

		Part	:NanomiteLightBurst.part
		Part	:NanomiteLightRays.part

		Part	:NanomiteLightRings.part
		Part	:NanomiteLightRings2.part
		
		Magnet	Root
		Lookat	Root
	End

End

Condition
	On	cycle
	Time	9
	Chance	.8

	Event	
		Ename	Light4
		Type	local
		At	Prime4
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:NanomiteLight.part
		Part	:NanomiteLightstar.part
		LifeSpan	3
	End
End
#############################################################################

Condition
	On	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy

	End
	
End

Condition
	On	Prime1Hit
	
	
	Event
		EName 	Prime1
		Type	Destroy

	End
	
End

Condition
	On	Prime2Hit
	
	
	Event
		EName 	Prime2
		Type	Destroy

	End
	
End

Condition
	On	Prime3Hit
	
	
	Event
		EName 	Prime3
		Type	Destroy

	End
	
End

Condition
	On	Prime4Hit
	
	Event
		EName 	Prime4
		Type	Destroy

	End

End

End		