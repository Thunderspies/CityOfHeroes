#############################################################
## MonitorSphere_Combat.fx
#############################################################

FxInfo

#############################################################

Condition
	On Cycle
	Time 10
	Event
		Type	Local
		At	Hips
		SoundNoRepeat 1
		Sound PPD_MonitorSphere_Combat_01 100 100 .35
		Sound PPD_MonitorSphere_Combat_02 100 100 .35
		Sound PPD_MonitorSphere_Combat_03 100 100 .35
		# Sound PPD_MonitorSphere_Combat_04 100 100 .35
		# Sound PPD_MonitorSphere_Combat_05 100 100 .35
		# Sound PPD_MonitorSphere_Combat_06 100 100 .35
		# Sound PPD_MonitorSphere_Combat_07 100 100 .35
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			Scale		0.99 0.99 0.99
			StartColor	255 0 0
		End
		Geom	GEO_MonitorSphereGlowBand
		Sound PPD_MonitorSphere_Combat_Loop 50 50 .5
	End

	Event
		EName	MonitorSphereAnchor
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Scale		1.1 1.1 1.1
			StartColor	255 0 0
		End
		Part	:Sphere_Light.part
		Geom	GEO_MonitorSphereGlowBand_Additive
	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
		Part	:Sphere_Light_AlwaysOn_Combat.part
	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
		Part	:Sphere_Light_AlwaysOn_Combat.part
	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	3
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sphere_Light_Combat.part
		Part	:Sphere_Light_Highlight_Combat.part
		Part	:Sphere_Light_AlwaysOn_Combat.part
	End
End

#########################################################################################


End


