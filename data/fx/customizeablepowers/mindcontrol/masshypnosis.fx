#############################################################
## MassHypnosis.fx
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	nexius
		Type	start
		At	root
		Geom	RootToHeadDummy
	End

	Event
		EName	corea
		Type	start
		At	nexius
		altpiv	1
		bhvr	:HypnoSpin.bhvr
		Geom	LocalPivot01
		Sound mental5b 80 80 0.75
		Lifespan 42
	End

	Event
		EName	coreaa
		Type	Local
		At	corea
		Altpiv	4
		Part	:MindControlRings.part
		Part	:MindControlRingz.part
		Lifespan 42
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	coreaaa
		Type	Local
		At	corea
		Altpiv	4
		Part	:MindGlowAOE.part
		Lifespan 40.25
	End
End

#############################################################

Condition
	On 	Time
	Time 	21

	Event
		EName	coreb
		Type	start
		At	nexius
		altpiv	1
		bhvr	:HypnoSpin.bhvr
		Geom	LocalPivot01
		Lifespan 42
	End

	Event
		EName	corebb
		Type	Local
		At	coreb
		Altpiv	4
		Part	:MindControlRings.part
		Part	:MindControlRingz.part
		Lifespan 42
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	corebbb
		Type	Local
		At	coreb
		Altpiv	4
		Part	:MindGlowAOE.part
		Lifespan 45.25
	End
End

#############################################################

Condition
	On 	Time
	Time 	32

	Event
		EName	corec
		Type	start
		At	nexius
		Altpiv	1
		bhvr	:HypnoSpin.bhvr
		Geom	LocalPivot01
		Lifespan 42
	End

	Event
		EName	corecc
		Type	Local
		At	corec
		Altpiv	4
		Part	:MindControlRings.part
		Part	:MindControlRingz.part
		Lifespan 42
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName	coreccc
		Type	Local
		At	corec
		Altpiv	4
		Part	:MindGlowAOE.part
		Lifespan 47.25
	End
End

#############################################################

Condition
	On 	Time
	Time 	43

	Event
		EName	cored
		Type	start
		At	nexius
		altpiv	1
		bhvr	:HypnoSpin.bhvr
		Geom	LocalPivot01
		Lifespan 42
	End

	Event
		EName	coredd
		Type	Local
		At	cored
		Altpiv	4
		Part	:MindControlRings.part
		Part	:MindControlRingz.part
		Lifespan 42
	End
End

Condition
	On 	Time
	Time 	48

	Event
		EName	coreddd
		Type	Local
		At	cored
		Altpiv	4
		Part	:MindGlowAOE.part
		Lifespan 45.25
	End
End

#############################################################

End