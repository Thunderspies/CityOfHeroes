#############################################################
## StranglerRoots.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	time
	Time 	0

	Event
		Ename	vines
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFadea.bhvr
		geom	StranglerVinebig04
		Sound tuatha_emerge 60 60 0.7
	End

	Event
		Ename	vines
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFadea.bhvr
		Part	ENEMYFX\Croatoa\Tuatha\VineDust.part
		Part	ENEMYFX\Croatoa\Tuatha\VineDust2.part
		Sound Tuatha_loop 60 60 0.66
	End
End

Condition
	On 	time
	Time 	5

	Event
		Ename	vines1
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFade1a.bhvr
		geom	StranglerVinebig03
	End
End

Condition
	On 	time
	Time 	7

	Event
		Ename	vines2
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFade2a.bhvr
		geom	StranglerVinebig02
	End
End

Condition
	On 	time
	Time 	12

	Event
		Ename	vines3
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\VineFade3a.bhvr
		geom	StranglerVinebig01
	End
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Ename	Hookup
		Type	posit
		At	root
		Bhvr	ENEMYFX\Croatoa\Tuatha\spin.bhvr
		geom	Centerdummy
	End
End

Condition
	On 	time
	Time 	0

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade4.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	2

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	3

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade4.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	8

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade3.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	11

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade1.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	15

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade4.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	20

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade1.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	23

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade3.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	24

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade3.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

Condition
	On 	time
	Time 	30

	Event
		Ename	vines
		Type	posit
		At	Hookup
		AltPiv	1
		Bhvr	ENEMYFX\Croatoa\Tuatha\StranglerStraightVineFade1.bhvr
		geom	StranglerStraightvine
		geom	StranglerStraightvineA
		geom	StranglerStraightvineB
		geom	StranglerStraightvine2
		geom	StranglerStraightvine2A
		geom	StranglerStraightvine2B
		geom	StranglerStraightvine3
		geom	StranglerStraightvine3A
		geom	StranglerStraightvine3B
	End
End

#############################################################

End