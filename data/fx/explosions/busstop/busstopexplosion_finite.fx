#########################################################
##	onfire
##
FxInfo

LifeSpan	800

#############  Sound  ############################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest

		#Sound	telekinetic3 80 30 .9

	End


	Event
		Ename	Base
		Type	Local
		BhvrOverride
			pyrRotate	0 90 0
		End

		At	Root

	End
End

############  Dust   ###############################


Condition
	On 	Time
	Time 	0
	Repeat	7

	Event
		Type	start
		At	Base

		BhvrOverride
			PositionOffset	0 4 0
			StartJitter	7 4 4
		End

		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

	End

End

############   Main     ################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Top
		Type	start
		At	Base
		Geom	exp_bustop_roof_top

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 8 0

			InitialVelocity		0 1 0
			InitialVelocityJitter	0.25 0.2 0.25

			SpinJitter		.2 .1 .05

			PhysDebris		0
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

	Event
		Ename	Bench
		Type	start
		At	Base
		Geom	exp_bustop_bench


		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1.5 -1
			scale			1 1 1

			InitialVelocity		0 0.8 .15
			InitialVelocityJitter	0.25 0.2 0.25

			Spin			.1 0 0
			SpinJitter		.2 .1 .1

			PhysDebris		0
			PhysDensity		5

		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	plank
		Type	start
		At	Base

		Geom	exp_bustop_plank


		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 2.5 -2

			InitialVelocity		0 0.75 -.3
			InitialVelocityJitter	0.25 0.25 0.25

			Spin			-.15 0 0
			SpinJitter		.2 .1 .05

			PhysDebris		0
			PhysDensity		5
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

End

#############  Supports  ############################


Condition
	On 	Time
	Time 	0

	Event
		Ename	LegLF
		Type	start
		At	Base

		Geom	exp_bustop_leg

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		6.9 4 2.25

			InitialVelocity		.4 0.25 .3
			InitialVelocityJitter	0.25 0.15 0.25

			Spin			.05 0 -.15
			SpinJitter		.2 .2 .2

			PhysDebris		0
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	LegRF
		Type	start
		At	Base

		Geom	exp_bustop_leg

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		-6.9 4 2.25

			InitialVelocity		-.4 0.25 .3
			InitialVelocityJitter	0.25 0.15 0.25

			Spin			.05 0 .15
			SpinJitter		.2 .2 .2
			PhysDebris		0

		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End


	Event
		Ename	LegLB
		Type	start
		At	Base

		Geom	exp_bustop_leg

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		6.9 4 -2.25

			InitialVelocity		.4 0.25 -.3
			InitialVelocityJitter	0.25 0.15 0.25

			Spin			-.05 0 -.15
			SpinJitter		.2 .2 .2

			PhysDebris		0

		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	1220
		LifespanJitter	30
	End


	Event
		Ename	LegRB
		Type	start
		At	Base

		Geom	exp_bustop_leg

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		-6.9 4 -2.25

			InitialVelocity		-.4 0.25 -.3
			InitialVelocityJitter	0.25 0.15 0.25

			Spin			-.05 0 .15
			SpinJitter		.2 .2 .2

			PhysDebris		0

		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

End

###################  Braces   ##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	MainBrace
		Type	start
		At	Base

		Geom	exp_Bustop_brace_long

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 1 -2.25

			InitialVelocity		0 0.5 -.8
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			-.2 0 0
			SpinJitter		.2 .1 .05

			PhysDebris		0
			PhysDensity		5
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	BraceL
		Type	start
		At	Base

		Geom	exp_Bustop_brace

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		6.9 1 0

			InitialVelocity		.5 0.5 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0 0 -.2
			SpinJitter		.1 .1 .2

			PhysDebris		0
			PhysDensity		4
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

	Event
		Ename	BraceR
		Type	start
		At	Base

		Geom	exp_Bustop_brace

		Bhvr	:DebrisFade.bhvr
		BhvrOverride
			PositionOffset		-6.9 1 0

			InitialVelocity		-.5 0.5 0
			InitialVelocityJitter	0.3 0.3 0.3

			Spin			0 0 .2
			SpinJitter		.1 .1 .2

			PhysDebris		0
			PhysDensity		4
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1220
		LifespanJitter	30
	End

End

#####################   Glass Panel 1  ##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	BackL
		Type	Start
		At	Base
		BhvrOverride
			PositionOffset		7 5 -2
		End
		LifeSpan 	3
		#Sound	Ice7 100 80 .6
		Part1	Explosions\GlassExplosion\GlassDust.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		3
	RepeatJitter	1

	Event
		Type	Start
		At	BackL
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanelExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		1

	Event
		Type	Start
		At	BackL
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanel.fx
	End

End

#####################   Glass Panel 2  ##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	BackR
		Type	Start
		At	Base
		BhvrOverride
			PositionOffset		-7 5 -2
		End
		LifeSpan 	3
		#Sound	Ice7 100 80 .6
		Part1	Explosions\GlassExplosion\GlassDust.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		3
	RepeatJitter	1

	Event
		Type	Start
		At	BackR
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanelExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		1

	Event
		Type	Start
		At	BackR
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanel.fx
	End

End

#####################   Glass Panel 3  ##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	BackC
		Type	Start
		At	Base
		BhvrOverride
			PositionOffset		0 5 -2
		End
		LifeSpan 	3
		#Sound	Ice7 100 80 .6
		Part1	Explosions\GlassExplosion\GlassDust.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
		Part1	Explosions\GlassExplosion\GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		3
	RepeatJitter	1

	Event
		Type	Start
		At	BackC
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanelExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		1

	Event
		Type	Start
		At	BackC
		LifeSpan 	100
		ChildFX	Explosions\GlassExplosion\GlassPanel.fx
	End

End

End