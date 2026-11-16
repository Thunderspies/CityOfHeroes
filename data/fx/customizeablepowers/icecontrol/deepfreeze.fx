#############################################################
## Cond Control - Chillblain
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	root
		Sound Ice7 100 100 .6
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	posit
		At	Root
		part1   :DeepFreezeShine.part
		geom	Dummy
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze.bhvr
		Sound Ice1_loop 100 100 .45
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	neck
		Part4	:DeepFreezeTargetMist.part
	End

	Event
		Type	Local
		At	chest
		Part2	:ChillTouchSnowA.part
		Part4	:DeepFreezeTargetMisttrail.part
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
		At	Hips
		part2   :DeepFreezeHitFlash.part
	End
End

### RING ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	posit
		At	Root
		part1	:DeepFreezeColdMist.part
		part2	:DeepFreezeColdMistFlat.part
		part3	:DeepFreezeIceChunks.part
	End
End

##############################################################

Condition
	On 	Time
	Time 	2
	Repeat 	4

	Event
		EName 	IceChunks
		Type	start
		At	Hips
		LifeSpan 	200
		ChildFX	:IceChunkScatter.fx
	End

	Event
		HardwareOnly
		EName 	IceChunks
		Type	start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	1 0 1
		End
		LifeSpan 	200
		ChildFX	:IceChunkExplode.fx
	End

	Event
		HardwareOnly
		EName 	IceChunks
		Type	start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
			StartJitter	1 0 1
		End
		LifeSpan 	200
		ChildFX	:IceChunkLargeScatter.fx
	End
End

##############################################################

Condition
	On 	Time
	Time 	4

	Event
		EName 	Icesickle01
		Type	posit
		At	Prime
		AltPiv	1
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_01
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Icesickle02
		Type	posit
		At	Prime
		AltPiv	2
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_02
	End
End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Icesickle03
		Type	posit
		At	Prime
		AltPiv	3
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_03
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName 	Icesickle04
		Type	posit
		At	Prime
		AltPiv	4
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_04
	End
End

Condition
	On 	Time
	Time 	8

	Event
		EName 	Icesickle05
		Type	posit
		At	Prime
		AltPiv	5
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_05
	End
End

Condition
	On 	Time
	Time 	9

	Event
		EName 	Icesickle06
		Type	posit
		At	Prime
		AltPiv	6
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_06
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Icesickle07
		Type	posit
		At	Prime
		AltPiv	7
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_07
	End
End

Condition
	On 	Time
	Time 	11

	Event
		EName 	Icesickle08
		Type	posit
		At	Prime
		AltPiv	8
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr

		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_08
	End
End

Condition
	On 	Time
	Time 	12

	Event
		EName 	Icesickle09
		Type	posit
		At	Prime
		AltPiv	9
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_09
	End
End

Condition
	On 	Time
	Time 	13

	Event
		EName 	Icesickle10
		Type	posit
		At	Prime
		AltPiv	10
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_10
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	Icesickle11
		Type	posit
		At	Prime
		AltPiv	11
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_11
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Icesickle12
		Type	posit
		At	Prime
		AltPiv	12
		Bhvr	behaviors/powers/coldcontrol/DeepFreeze01.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_FX_IceWall_12
	End
End

#############################################################

End