#############################################################
## SnowBody.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  Neck
	End

	Event
		Type	Local
		At	SpadL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  Hips
	End

	Event
		Type	Local
		At	SpadR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  Hips
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  LarmL
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  WepR
	End

	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther	WepL
	End

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  LlegL
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  LlegR
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  LlegL
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ColdMist.part
		POther  LlegR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	UlegL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:Snowflakes.part
	End
End

#############################################################

End