#############################################################
## Header
#############################################################

FxInfo

LifeSpan 200

Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	Origin
		Sound illusiontoss3 60 60 .6
		End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Prime
		Type	start
		At	wepr
		geom	FX_ElectricityCageTops
		bhvr	behaviors/GadgetCageLobbprojectile.bhvr

		Magnet	T_Root
		LookAt	T_Root

	End

	Event

		Type	local
		At	Prime
		Altpiv	2
		Part	Powers/ElectricityControl/GadgetCageElectricity.part
		Part	Powers/ElectricityControl/GadgetCageElectricityLarge.part
		Part	Powers/ElectricityControl/ElectricityInnerGlowLocal.part
		Part	Powers/ElectricityControl/ElectricityOuterGlowLocal.part
	End
End

##################################


Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End


End


End