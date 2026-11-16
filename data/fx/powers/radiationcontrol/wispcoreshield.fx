#########################################################
##	RadiationAura
FxInfo

##################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	origin
		Sound glow2b_loop 70 70 .4
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
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

End

End				