#############################################################
## CoTAltar_Death.fx
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Start
		At	Root
		BHVR	Behaviors\VaultCameraShake.bhvr
		Sound HugerockExplo2 110 110 0.97
		LifeSpan 300
	End
End

Condition
	On	Time
	Time 	0

	Event
		EName	AltarOffset
		Type	Start
		At	Root
		Geom	cot_altar_round_sml_destroyed
	End
End

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At	AltarOffset
		AltPiv	1
		Part	Explosions\Huge_Rock\InfernoCare.part
		Part	Explosions\Huge_Rock\Inferno.part
		Part	Explosions\Huge_Rock\Sparks.part
		LifeSpan 9

	End

	Event
		Type	Local
		At	AltarOffset
		AltPiv	1
		Part	Explosions\Huge_Rock\InfernoExplosion.part
		Part	Explosions\Huge_Rock\SparksSmall.part
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	AltarOffset
		AltPiv	1
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		LifeSpan 4
	End
End

Condition
	Time 	1
	Repeat	12

	Event
		Type	start
		At	AltarOffset
		AltPiv	1
		Bhvr	Explosions\Huge_Rock\Phys1.bhvr
		Part1	powers\firecontrol\fire.part
		Part1	powers\firecontrol\fire.part
		Lifespan 60
		Lifespanjitter 20
	End

	Event
		HardwareOnly
		Type	start
		At	AltarOffset
		AltPiv	1
		Bhvr	Explosions\Huge_Rock\Phys1.bhvr
		Part1	powers\firecontrol\fire.part
		Part1	powers\firecontrol\fire.part
		Lifespan 60
		Lifespanjitter 20
	End

	Event
		Type 	start
		At	AltarOffset
		AltPiv	1
		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
		Lifespanjitter 20
	End

	Event
		HardwareOnly
		Type 	start
		At	AltarOffset
		AltPiv	1
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 20
	End
End

######################################################################################################

Condition
	On 	Time
	Time 	0
	Repeat	6

	Event
		Type	start
		At	AltarOffset
		AltPiv	1
		Bhvr 	Explosions\Huge_Rock\VaultPhys.bhvr
		BhvrOverride
			StartJitter	6 6 2
			Scale		0.25 0.25 0.25
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		CThresh 0.1
		Lifespan 200
	End

	Event
		Type	start
		At	AltarOffset
		AltPiv	1
		Bhvr 	Explosions\Huge_Rock\VaultPhys.bhvr
		BhvrOverride
			StartJitter	6 6 2
			Scale		0.3 0.3 0.3
		End
		At	AltarOffset
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		CThresh 0.1
		Lifespan 200
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	12

	Event
		HardwareOnly
		Type	start
		At	AltarOffset
		AltPiv	1
		Bhvr 	Explosions\Huge_Rock\VaultPhys.bhvr
		BhvrOverride
			StartJitter	6 6 2
			Scale		0.2 0.2 0.2
		End
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		CThresh 0.1
		Lifespan 200
	End

	Event
		HardwareOnly
		Type	start
		At	AltarOffset
		AltPiv	1
		Bhvr 	Explosions\Huge_Rock\VaultPhys.bhvr
		BhvrOverride
			StartJitter	6 6 2
			Scale		0.15 0.15 0.15
		End
		At	AltarOffset
		Geom	Cav_rockSM_01
		Geom	Cav_rockSM_02
		Geom	Cav_rockSM_03
		CThresh 0.1
		Lifespan 200
	End
End

#############################################################

End