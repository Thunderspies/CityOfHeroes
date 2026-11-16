#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Eyes
		sound	peaceeyes1 60 50 .7
		LifeSpan	3
	End
End


Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	Eyes
		sound	obsidian_loop 60 50 .2
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	CoreOffset_Clockwise1
		Type	Local
		At	root
		Bhvr	:Lightform.bhvr
	End

	Event
		EName	core
		Type	Local
		At	CoreOffset_Clockwise1
		Bhvr	:LightformA.bhvr
		Part	:LightTendrils.part
		Part	:Sparks.part
	End

	Event
		EName	spinpiv
		Type	Local
		At	core
		Bhvr	:LightformSpin.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	CoreOffset_Clockwise2
		Type	Local
		At	root
		Bhvr	:Lightform2.bhvr
	End

	Event
		EName	spinpivUp
		Type	Local
		At	CoreOffset_Clockwise2
		Bhvr	:LightformSpinUp.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	CoreOffset_Clockwise3
		Type	Local
		At	root
		Bhvr	:Lightform2.bhvr
	End

	Event
		EName	spinpivUpb
		Type	Local
		At	CoreOffset_Clockwise3
		Bhvr	:LightformSpinUp.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	CoreOffset_CounterClockwise1
		Type	Local
		At	root
		Bhvr	:Lightform1.bhvr
	End

	Event
		EName	spinpiv1
		Type	Local
		At	CoreOffset_CounterClockwise1
		Bhvr	:LightformSpin1.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	CoreOffset_CounterClockwise2
		Type	Local
		At	root
		Bhvr	:Lightform1.bhvr
	End

	Event
		EName	spinpiva
		Type	Local
		At	CoreOffset_CounterClockwise2
		Bhvr	:LightformSpin1.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	CoreOffset_CounterClockwise4
		Type	Local
		At	root
		Bhvr	:Lightform3.bhvr
	End

	Event
		EName	spinpivdwna
		Type	Local
		At	CoreOffset_CounterClockwise4
		Bhvr	:LightformSpindwn.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
	End
End

#############################################################

End