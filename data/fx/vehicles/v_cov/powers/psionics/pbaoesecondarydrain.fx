#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	100

###########################################################
Condition
	On	Time
	Time	10

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

	Event
		Type	local
		At	WepL
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan	30	

	End
	
	Event
		Type	local
		At	WepR
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part	:EnergyHandGlows.part
		LifeSpan	30	

	End
End

############################################################################################3

Condition
	On	Time
	Time	0

	Event
		EName	Headnode
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment
		LifeSpan	20		
	End

	Event
		EName	coreR
		Type	start
		At	Headnode
		altpiv	1
		Part	:EnergyDown.part
		LifeSpan	20
	End

End
	

Condition
	On	Time
	Time	30
	
	Event
		EName	splatHookup
		Type	Local
		At	Root
		BHVR	:Spin.bhvr
		Geom	CenterDummy

		LifeSpan	15
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
		Bhvr	:Splat.bhvr
		#Part	:EnergyFlash.part
		LifeSpan	5
	End

End

Condition
	On	Time
	Time	30

	Event
		EName	coreR
		Type	start
		At	Root
		Part	:EnergyUp.part
		Part	:EnergyUp2.part
		LifeSpan	15
	End

End

##############################################################################

Condition
	On	Time
	Time	32

	
	Event
		EName	splatHookup
		Type	Local
		At	Root
		BHVR	:SplatRings.bhvr
		Splat	Generic_Ring


		LifeSpan	10
	End


End

Condition
	On	Time
	Time	36

	
	Event
		EName	rootnode
		Type	local
		At	Root
		
		BHVR	:SplatRings2.bhvr
		Splat	Generic_Ring
		#Part	:LightEnergyOutBurst.part

		LifeSpan	10
	End

End

Condition
	On	Time
	Time	31
	
	Event

		Ename	Splat	
		Type	Destroy

	End	

End

End
