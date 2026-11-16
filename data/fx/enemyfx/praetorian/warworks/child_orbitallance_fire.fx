#############################################################
## Child_OrbitalLance_Attack.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	PlasmaAnchorR
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.0 0.7 0.0
		End
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Tendril.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Tendril.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

Condition
	On	Time
	Time	36

	Event
		Type	Start
		At	PlasmaAnchorR
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

#############################################################

End