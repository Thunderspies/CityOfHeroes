#########################################################
##	forceBubble
########################################################
FxInfo

Condition
	On	cycle
	Time	20

	Event	
		Type	Local
		At	Root
		
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_Shake.bhvr
		LifeSpan	10
	End

End

Condition
	On	cycle
	Time	10
	Chance	.7

	Event	
		Type	Local
		At	EmitterStatic
		
		Childfx	V_COV\enemypowers/arachnos/ItemofPowerLightning.fx
		LifeSpan	40
	End

End

Condition
	On	Time
	Time	0

	Event	
		EName	EmitterStatic
		Type	Local
		At	Root
		bhvrOverride
			behavior
			PositionOffset		0 11.968 0
		End
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_Shake.bhvr
	End


	Event
		EName	Emitter
		Type	Local
		At	Root
		bhvrOverride
			behavior
			PositionOffset		0 11.968 0
			Scale			5 5 5
			Spin			.1 .1 .1
			SpinJitter		1 1 1
		End
#		Geom	Testing_TargetB
	End

	Event	
		ENAME	HeadNode
		Type	Local
		At	Emitter
		Bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_Offset.bhvr

	End

	Event	
		EName	Emitter01
		Type	Local
		At	EmitterStatic
		Part	:True_Furnace_LightRays.part

		#Pmagnet Emitter
	End

	Event	
		EName	Emitter01
		Type	Local
		At	EmitterStatic

		part	V_COV\BaseFx\ItemsOfPower\True_Furnace_P_Glow01.part
		#Pmagnet Emitter
	End
End

#################################################################################


Condition
	On	Time
	Time	1

	Event
		EName	SpecsSide01
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End

	Event
		EName	SpecsSide02
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_Child01.bhvr
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End

	Event
		EName	SpecsSide03
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_Child02.bhvr
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End

	Event
		EName	SpecsSide04
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_Child03.bhvr
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End

	Event
		EName	SpecsEmitTop
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_ChildU.bhvr
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End

	Event
		EName	SpecsEmitTop
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_ChildU01.bhvr
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End

	Event
		EName	SpecsBot
		Type	Local
		At	EmitterStatic

#		Geom	Testing_TargetB
		bhvr	V_COV\BaseFx\ItemsOfPower\True_Furnace_B_ChildB.bhvr
		Childfx	V_COV\BaseFx\ItemsOfPower\Furnace_ChildSpecks.fx
	End


	Event
		EName	SpecsBot01
		Type	Local
		At	EmitterStatic
		bhvrOverride
			behavior
			PositionOffset		-6.261 -5.953 0
		End
#		Geom	Testing_TargetB
	End

	Event
		EName	SpecsTop01
		Type	Local
		At	EmitterStatic
		bhvrOverride
			behavior
			PositionOffset		-6.261 5.953 0
		End
#		Geom	Testing_TargetB
	End

	Event
		EName	Specs01
		Type	Local
		At	SpecsBot01
		
		Part1	V_COV\BaseFx\ItemsOfPower\True_Furnace_P_Speck.part
		PMagnet	Emitter
		POther	SpecsTop01
	End

End



End


