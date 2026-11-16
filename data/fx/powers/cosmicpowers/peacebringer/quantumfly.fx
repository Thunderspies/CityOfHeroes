#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0

	Event
		EName	core
		Type	Local
		At	root
		Bhvr	:Lightform.bhvr
		#Part	:Light.part
		Part	:LightTendrils.part
		Part	:Sparks.part
		
	End

	Event
		EName	core2
		Type	Local
		At	root
		Bhvr	:Lightform2.bhvr
		
	End
	
	Event
		EName	core2b
		Type	Local
		At	root
		Bhvr	:Lightform2.bhvr
		
	End

	Event
		EName	spinpiv
		Type	Local
		At	core
		Bhvr	:LightformSpin.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End

	Event
		EName	spinpivUp
		Type	Local
		At	core2
		Bhvr	:LightformSpinUp.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End

	Event
		EName	spinpivUpb
		Type	Local
		At	core2b
		Bhvr	:LightformSpinUp.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End
##########################################################################
	Event
		EName	coreopposite
		Type	Local
		At	root
		Bhvr	:Lightform1.bhvr
		
	End

	Event
		EName	coreoppositea
		Type	Local
		At	root
		Bhvr	:Lightform1.bhvr
		
	End

	Event
		EName	coreopposite2
		Type	Local
		At	root
		Bhvr	:Lightform3.bhvr
		
	End

	
	Event
		EName	coreopposite2a
		Type	Local
		At	root
		Bhvr	:Lightform3.bhvr
		
	End

	Event
		EName	spinpiv1
		Type	Local
		At	coreopposite
		Bhvr	:LightformSpin1.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End

	Event
		EName	spinpiva
		Type	Local
		At	coreoppositea
		Bhvr	:LightformSpin1.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End

	Event
		EName	spinpivdwna
		Type	Local
		At	coreopposite2a
		Bhvr	:LightformSpindwn.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End

		Event
		EName	spinpivdwna
		Type	Local
		At	coreopposite2a
		Bhvr	:LightformSpindwn.bhvr
		part	:LightformCore.part
		part	:LightformEnergy.part
		
	End

End

End

