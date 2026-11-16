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
		EName	PlasmaAnchorL
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.0 0.7 0.0
		End
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Start
		At	PlasmaAnchorL
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
	Time	13

	Event
		Type	Start
		At	PlasmaAnchorL
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
	Time	19

	Event
		Type	Start
		At	PlasmaAnchorL
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
	Time	25

	Event
		Type	Start
		At	PlasmaAnchorL
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
	Time	31

	Event
		Type	Start
		At	PlasmaAnchorL
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
	Time	37

	Event
		Type	Start
		At	PlasmaAnchorL
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
	Time	43

	Event
		Type	Start
		At	PlasmaAnchorL
		BhvrOverride
			InitialVelocity		0.0 5.0 0.0
			InitialVelocityJitter	1.0 0.0 1.0
		End
		Part	:OrbitalLance_Head_Ember_Small.part
		Part	:OrbitalLance_Trail_Glow.part
		Part	:Projectile_Head_Ember.part
	End
End

#Condition
#	On	Time
#	Time	49
#
#	Event
#		Type	Start
#		At	PlasmaAnchorL
#		BhvrOverride
#			InitialVelocity		0.0 5.0 0.0
#			InitialVelocityJitter	0.1 0.0 0.1
#		End
#		Part	:Projectile_Head_Ember_Small.part
#		Part	:OrbitalLance_Trail_Tendril.part
#		Part	:OrbitalLance_Trail_Glow.part
#		Part	:Projectile_Head_Ember.part
#	End
#End
#############################################################

End