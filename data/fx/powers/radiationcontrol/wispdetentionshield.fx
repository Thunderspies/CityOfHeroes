#########################################################
##	RadiationAura
FxInfo

##################################


Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound leaderglow3 80.0 80.0 .55
	End

End





Condition
	On	Time	
	Time 	8

	Event
		Type 	Local
		At	Origin
		Sound glow2b_loop 80.0 80.0 .43
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 290
	End

End






Condition
	On	Time
	Time	0
	
	Event
		
		Type	Local
		At	Hair
		BHVR	Behaviors/OffsetCrystal.bhvr
		
		part	EnemyFx/Rularuu/wisp/Buffs/GlowShield.Part
		LifeSpan	4
	End

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/WispShield.bhvr
		Geom	WispShield
				
	End

	Event
		Type	local
		At	Root
		
		Bhvr 	behaviors/PivotOffsetChest.bhvr
		part	:WispDetentionShieldRings.part
		part	:WispDetentionShieldRingsUp.part
		part	:WispDetentionShieldRingsDown.part
		
				
	End

End

End				