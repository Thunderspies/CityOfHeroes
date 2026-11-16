#########################################################
##	FireAura
FxInfo
Lifespan 600

##################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At 	Origin
#		Sound	ignite2 80 30 .9

	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Hips
		Type	Local
		At	Hips
		Pmagnet Head
		BhvrOverride
			Scale 5.0 5.0 5.0
		End

		part	:TorchflameAdditive.part
		part	:TorchflameSub2.part

	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		Pmagnet Head
		BhvrOverride
			StartJitter 0.0 1.0 0.0
			Scale 6.5 6.5 6.5
		End

		part	:TorchflameAdditive.part
		part	:TorchflameHighlight.part
		part	:TorchflameSubTrail2.part


	End

	Event
		EName	Neck
		Type	Local
		At	Neck
		BhvrOverride
			Scale 1.5 1.5 1.5
		End

		part	:TorchflameAdditive.part
		part	:TorchflameHighlight.part
		part	:TorchflameSubTrail2.part
		part	:TorchflameAdditiveTrail.part

	End
End



