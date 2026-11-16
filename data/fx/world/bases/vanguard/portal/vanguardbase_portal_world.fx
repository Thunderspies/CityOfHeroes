#########################################################
## Vanguard Base Portal (modified from Superbase Portal FX)
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	gravWell
		Type	Start
		At	Root
		Magnet	gravWell
		part	:VanguardBase_Portal_Mist01.part
		part	:VanguardBase_Portal_Mist02.part
		part	:VanguardBase_Portal_Sparks01.part
	End

	Event
		EName 	Pulse
		Type	Start
		At	gravWell
		childFX	:VanguardBase_Portal_Pulse.fx
	End

	Event
		EName 	Core
		Type	Start 
		At	GravWell
		BhvrOverride
			PositionOffset 0 .1 0
		End
		part	:VanguardBase_Portal_Core01.part
		part	:VanguardBase_Portal_OutterRing01.part
	End

	Event
		EName 	Rings
		Type	Start 
		At	Core
		part	:VanguardBase_Portal_InnerRing01.part
		part	:VanguardBase_Portal_InnerRing02.part
	End

	Event
		EName 	Center
		Type	Start 
		At	Root
		part	:VanguardBase_Portal_Rift01.part
		part	:VanguardBase_Portal_Rift02.part
	End

	Event
		EName 	Spinner
		Type	Start
		At	Root
		Bhvr	:VanguardBase_Portal_RiftSpinner.bhvr
	End

	Event
		EName 	helix
		Type	Local 
		At	Spinner
		Bhvr	:VanguardBase_Portal_RiftOffset.bhvr
		part	:VanguardBase_Portal_Sparks02.part
		part	:VanguardBase_Portal_Glow01.part
	End

##	Event
##		EName 	helix2
##		Type	Local 
##		At	Spinner
##		Bhvr	:VanguardBase_Portal_RiftOffset2.bhvr
##		part	:VanguardBase_Portal_Sparks02.part
##	End

	Event
		EName 	Spinner2
		Type	Start
		At	Root
		Bhvr	:VanguardBase_Portal_RiftSpinner2.bhvr
	End

	Event
		EName 	helix3
		Type	Local 
		At	Spinner2
		Bhvr	:VanguardBase_Portal_RiftOffset.bhvr
		part	:VanguardBase_Portal_Sparks02.part
		part	:VanguardBase_Portal_Glow01.part
	End

##	Event
##		EName 	helix4
##		Type	Local 
##		At	Spinner2
##		Bhvr	:VanguardBase_Portal_RiftOffset2.bhvr
##		part	:VanguardBase_Portal_Sparks02.part
##	End
End

#########################################################

End