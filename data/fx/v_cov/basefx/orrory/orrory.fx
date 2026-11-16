#########################################################
##	template

FxInfo

Lighting  1


#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound magiccalder_loop 120 120 1.0
	End
End


Condition
	On	Time
	Time	0

	Event
		Type Local
		At origin
		Sound Sign6_loop 90 90 .2
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0


	Event	
		ENAME	Center
		Type	Local
		At	Root
	End
#################### Globe 1 ########################
	
	Event	
		ENAME	CenterSpin
		Type	Local
		At	Root
		Geom	_basecntrlfx_globe01a_frame
		Bhvr	\behaviors\Bases\Orrory\OrroryCenterSpin01.bhvr
	
	End

	Event	
		ENAME	Globe1
		Type	Local
		At	CenterSpin
		Geom	_basecntrlfx_globe01a__basecntrlfxbright
	
	End

#################### Globe 2 ########################

	Event	
		ENAME	CounterSpin1
		Type	Local
		At	Center
		Bhvr	\behaviors\Bases\Orrory\OrroryCounterSpin01.bhvr
	
	End
	
	Event	
		ENAME	Globe2Frame
		Type	Local
		At	CounterSpin1
		Geom	_basecntrlfx_globe02b_framebase
		
	End

	Event	
		ENAME	Globe2Offset
		Type	Local
		At	CounterSpin1
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe2Offset.bhvr
	End
	
	Event	
		ENAME	Globe2
		Type	Local
		At	Globe2Offset
		Geom	_basecntrlfx_globe02a
	End

	Event	
		ENAME	Globe2Frame
		Type	Local
		At	Globe2Offset
		Geom	_basecntrlfx_globe02a_frame__basecntrlfx
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe2FrameSpin.bhvr
	End
#########################Globe 2a##############################



	Event	
		ENAME	Globe2bOffset
		Type	Local
		At	CounterSpin1
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe2bOffset.bhvr
	End

	Event	
		ENAME	Globe2b
		Type	Local
		At	Globe2bOffset
		Geom	_basecntrlfx_globe02b
	End

	Event	
		ENAME	Globe2bFrame
		Type	Local
		At	Globe2bOffset
		Geom	_basecntrlfx_globe02a_frame__basecntrlfx
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe2bFrameSpin.bhvr
	End


#########################Globe 3a##############################

	Event	
		ENAME	CounterSpin2
		Type	Local
		At	Center
		Bhvr	\behaviors\Bases\Orrory\OrroryCounterSpin02.bhvr
	
	End

	Event	
		ENAME	Globe3aOffset
		Type	Local
		At	CounterSpin2
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3aOffset.bhvr
	End

	Event	
		ENAME	Globe3bOffset
		Type	Local
		At	CounterSpin2
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3bOffset.bhvr
	End

		Event	
		ENAME	Globe3cOffset
		Type	Local
		At	CounterSpin2
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3cOffset.bhvr
	End



	Event	
		ENAME	Globe3a
		Type	Local
		At	Globe3aOffset
		Geom	_basecntrlfx_globe03a
	End


	Event	
		ENAME	Globe3b
		Type	Local
		At	Globe3bOffset
		Geom	_basecntrlfx_globe03b
	End

	Event	
		ENAME	Globe3c
		Type	Local
		At	Globe3cOffset
		Geom	_basecntrlfx_globe03c
	End



	Event	
		ENAME	Globe3bbase
		Type	Local
		At	CenterSpin
		Geom	_basecntrlfx_globe03a_framebase
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3abase.bhvr
	End

	Event	
		ENAME	Globe3aFrame
		Type	Local
		At	Globe3aOffset
		Geom	_basecntrlfx_globe03a_frame__basecntrlfx
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3aFrameSpin.bhvr
	End

	Event	
		ENAME	Globe3bFrame
		Type	Local
		At	Globe3bOffset
		Geom	_basecntrlfx_globe03a_frame__basecntrlfx
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3bFrameSpin.bhvr
	End

	Event	
		ENAME	Globe3cFrame
		Type	Local
		At	Globe3cOffset
		Geom	_basecntrlfx_globe03c_frame__basecntrlfx
		Bhvr	\behaviors\Bases\Orrory\OrroryGlobe3aFrameSpin.bhvr
	End



End


End		
