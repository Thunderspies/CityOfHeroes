#############################################################
## StranglerRoots.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	vines
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFadea.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerVinebig04
		Sound tuatha_emerge 60 60 0.7
	End

	Event
		Ename	vines
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFadea.bhvr
		Part	:VineDust.part
		Part	:VineDust2.part
		Sound Tuatha_loop 60 60 0.66
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Ename	vines1
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFade1a.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerVinebig03
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Ename	vines2
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFade2a.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerVinebig02
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Ename	vines3
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFade3a.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerVinebig01
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\spin.bhvr
		Geom	Centerdummy
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade4.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade4.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade3.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	11

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade4.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	23

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade3.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade3.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade1.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_StranglerStraightvine
		Geom	Tintable_StranglerStraightvineA
		Geom	Tintable_StranglerStraightvineB
		Geom	Tintable_StranglerStraightvine2
		Geom	Tintable_StranglerStraightvine2A
		Geom	Tintable_StranglerStraightvine2B
		Geom	Tintable_StranglerStraightvine3
		Geom	Tintable_StranglerStraightvine3A
		Geom	Tintable_StranglerStraightvine3B
	End
End

#############################################################

End