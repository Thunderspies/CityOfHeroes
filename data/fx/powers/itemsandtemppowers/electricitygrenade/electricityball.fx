#############################################################
## ElectricityBall.fx
#############################################################

FxInfo

LifeSpan 200

########################################################

Condition
	On	Time
	Time	10

	Event
		Type	local
		At	wepR
		Sound grtoss2 60 60 .6
	End

	Event
		Type	local
		At	wepR
		geom	grenade
		bhvr	behaviors\grenadeFade.bhvr
		lifespan 22
	End
End

#############################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	wepR
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		geom	grenade
		Magnet	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Type	Local
		At	T_Root
		Part	:ChainLightningArchs3.part
		Part	:ChainLightningArchs2.part
		Part	:ChainLightningArchs1.part
		Part	:ChainLightningArchs.part
		Part	:ChainLightningArchsSpinner.part
		Part	:ChainLightningRing2.part
		Part	:ChainLightningArchsInverted.part
		Sound Grexplo1 70 70 .77
		lifespan 10
	End
End

#############################################################

End