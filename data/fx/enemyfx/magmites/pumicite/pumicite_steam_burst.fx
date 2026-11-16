#########################################################
##	Template

FxInfo

Condition
	On 	cycle
	Time	100
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	Head
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetEye.bhvr
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	105
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	UArmR
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetUArmR.bhvr
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	110
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	Chest
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetChest.bhvr
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	90
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	Chest
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetBack.bhvr
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	70
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	ULegR
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetULegR.bhvr
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	80
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	LLegL
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetLLegL.bhvr
		lifespan	15
	End
End

Condition
	On 	cycle
	Time	30
	Chance	0.5

	Event
		EName 	SnortR
		Type	Local 
		At	UArmL
		part1	:GasBurstCloud.part
		part1	:GasBurstRoot.part
		bhvr	:OffsetUArmL.bhvr
		lifespan	15
	End
End

