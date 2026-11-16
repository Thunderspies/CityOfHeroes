#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	100

###########################################################
Condition
	On	Time
	Time	0

	Event
		EName	rootnodeL
		Type	local
		At	WepL
		BHVR	:ringSpin.bhvr
		Geom	centerdummy

	End

	Event
		EName	rootnodeR
		Type	local
		At	WepR
		BHVR	:ringSpin.bhvr
		Geom	centerdummy

	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	:ringSpin.bhvr
		Part	:SpinningHandRings.part
		Part	:SpinningHandRings.part

	End
	
	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	:ringSpin.bhvr
		Part	:SpinningHandRings.part
		Part	:SpinningHandRings.part

	End

	Event
		Type	local
		At	rootnodeL
		altpiv	1
		BHVR	:ringSpin.bhvr
		Part	:SpinningHandRings.part
		Part	:SpinningHandRings.part

	End
	
	Event
		Type	local
		At	rootnodeR
		altpiv	1
		BHVR	:ringSpin.bhvr
		Part	:SpinningHandRings.part
		Part	:SpinningHandRings.part

	End

End

############################################################################################3

Condition
	On	Time
	Time	0

	Event
		EName	rootnode
		Type	local
		At	Root
		Geom	RootToChestAdjustment

	End

	Event
		Type	start
		At	rootnode
		Altpiv	1
		Part	:EnergyOuta.part
		Part	:EnergyOut2a.part
		LifeSpan	18

	End

	Event
		EName	coreR
		Type	start
		At	Root
		Splat	SharpStar
		Bhvr	:Splat.bhvr
		#Part	:EnergyFlash.part
		LifeSpan	7
	End

End

Condition
	On	cycle
	Time	4

	Event
		EName	splat
		Type	start
		At	splatHookup

		Splat	ThinRing.tga
		Bhvr	:SplatRings3.bhvr
		
		LifeSpan	8
	End

	Event
		EName	splat
		Type	start
		At	splatHookup

		Splat	Generic_Ring.tga
		Bhvr	:SplatRings3.bhvr

		LifeSpan	8
	End

End

####################################################################################################

Condition
	On	Time
	Time	10
	
	Event
		EName	splatHookup
		Type	Local
		At	Root
		BHVR	:Spin.bhvr
		Geom	CenterDummy

		LifeSpan	11
	End

End

Condition
	On	cycle
	Time	1

	Event
		EName	splat
		Type	start
		At	splatHookup

		Splat	SharpStar
		Bhvr	:Splat2.bhvr
		#Part	:EnergyFlash.part
		LifeSpan	4
	End

End

######################################################################################################

Condition
	On	Time
	Time	24
	
	Event
		EName	s1
		Type	Local
		At	Root
		BHVR	:SplatRings4.bhvr
		Splat	Generic_Ring


		LifeSpan	10
	End

End

Condition
	On	Time
	Time	28

	Event
		EName	s2
		Type	Local
		At	Root
		BHVR	:SplatRings4.bhvr
		Splat	Generic_Ring


		LifeSpan	10
	End

End

End			