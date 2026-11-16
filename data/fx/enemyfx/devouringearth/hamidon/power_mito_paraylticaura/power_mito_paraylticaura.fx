#########################################################
## Mitochondria - Paralytic Aura
########################################################

FxInfo

########################################################

Condition
	On	Time
	Time 	17

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On	Time
	Time 	0

	Event
		EName 	Prime2
		Type	PositOnly 
		At	HIPS
		Bhvr 	:ParalyticAura.bhvr
		Geom	FX_ParalyticAura
		#LifeSpan	50
	End
End

Condition
	On	Time
	Time 	0

	Event
		EName 	Prime
		Type	PositOnly
		At	HIPS
		BhvrOverride
			StartJitter		0.0 0.0 0.0
			InitialVelocity		0.0 0.0 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Gravity			0.00000 
			Spin			0.000 -0.005 0.000000
			SpinJitter		0.000 0.0002 0.0000
			Alpha			255 
		End
		Part1	:ParalyticAura_Motes.part
		Part2	:ParalyticAura_Flashes.part
		Part3	:ParalyticAura_Flashes2.part
		Part4	:ParalyticAura_Flashes3.part
	End
End

########################################################

End