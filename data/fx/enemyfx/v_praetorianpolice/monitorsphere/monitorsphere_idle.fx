#############################################################
## MonitorSphere_Combat.fx
#############################################################

FxInfo

#############################################################

Condition
	On Cycle
	Time 50
	Event
		Type	Local
		At	Root
		# SoundNoRepeat 3
		Sound PPD_MonitorSphere_Idle_01 100 100 .35
		Sound PPD_MonitorSphere_Idle_02 100 100 .35
		Sound PPD_MonitorSphere_Idle_03 100 100 .35
		# Sound PPD_MonitorSphere_Idle_04 100 100 .35
		# Sound PPD_MonitorSphere_Idle_05 100 100 .35
		# Sound PPD_MonitorSphere_Idle_06 100 100 .35
		# Sound PPD_MonitorSphere_Idle_07 100 100 .35
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
			Scale		0.95 0.95 0.95

			StartColor	100 100 255
		End
		Geom	GEO_MonitorSphereGlowBand
		Sound PPD_MonitorSphere_Idle_Loop 50 50 .1
	End

	Event
		EName	MonitorSphereAnchor
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
		#	Scale		1.0 1.0 1.0
			StartColor	10 150 255
		End
		Part	:Sphere_Light.part
		Geom	GEO_MonitorSphereGlowBand_Additive
	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sphere_Light_AlwaysOn.part

		Part	:Sphere_Light.part
		Part	:Sphere_Light_Highlight.part
	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:Sphere_Light_AlwaysOn.part
		Part	:Sphere_Light.part
		Part	:Sphere_Light_Highlight.part

	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	3
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sphere_Light_AlwaysOn.part
		Part	:Sphere_Light.part
		Part	:Sphere_Light_Highlight.part

	End
End



#########################################################################################


End


