#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 		Time
	Time		0
	DoMany		1
	Event
		ENAME	Cloud
		Type	PositOnly
		At	Origin
		Flags	OneAtATime
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Cracks01.part
		Part	:Cracks02.part
		Part	:Cracks03.part
		Part	:Cracks04.part
		Part	:Cracks05.part
		Part	:Crack_Faultline.part
		Part	:Crackstrail_DustCloud.part
		Part	:Crackstrail_DustCloud2.part
		part	:Crackstrail_Rocks.part
	End


End

#########################################################

End