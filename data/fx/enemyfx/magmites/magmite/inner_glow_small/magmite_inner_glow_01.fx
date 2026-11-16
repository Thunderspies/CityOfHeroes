#########################################################
##	template

FxInfo

Flags InheritAlpha

Condition
	On 	Time
	Time 	0	

#####################  HEAD  ############################

	Event	
		ENAME	Head
		Type	Local
		At	Head
		part	:Lava_Head.part
		Part	:Inner_Glow_Head_01.part
	End

	Event	
		ENAME	Head
		Type	Local
		At	Neck
		part	:Lava_Head.part
		Part	:Inner_Glow_Head_01.part
	End

	Event	
		ENAME	EyeR
		Type	Local
		At	Head
		Part	:Inner_Glow_Eye.part
		Bhvr	:Head_EyeR_Offset.bhvr
	End

	Event	
		ENAME	EyeL
		Type	Local
		At	Head
		Part	:Inner_Glow_Eye.part
		Bhvr	:Head_EyeL_Offset.bhvr
	End

	Event	
		ENAME	Mouth
		Type	Local
		At	Head
		Part	:Inner_Glow_Mouth.part
		Bhvr	:Head_Mouth_Offset.bhvr
	End

####################  UArmR  ###########################

	Event	
		ENAME	LArmR
		Type	Local
		At	LArmR	
	End

	Event	
		ENAME	UArmR
		Type	Local
		At	UArmR
		part	:Lava_UArmR_01.part
		part	:Inner_Glow_UArmR_01.part
		Pmagnet LArmR
		POther  LArmR
	End

####################  LArmR  ###########################

	Event	
		ENAME	Hand_R
		Type	Local
		At	HandR	
	End

	Event	
		ENAME	LArmR
		Type	Local
		At	LArmR
		part	:Lava_LArmR_01.part
		part	:Inner_Glow_LArmR_01.part
		Pmagnet HandR
		POther  HandR
	End


####################  HandR  ###########################

	Event	
		ENAME	HandR
		Type	Local
		At	HandR
		part	:Lava_HandR.part
		Part	:Inner_Glow_HandR.part
		Bhvr	:HandR_Offset_01.bhvr
	End

	Event	
		ENAME	HandR
		Type	Local
		At	HandR
		part	:Lava_HandR.part
		Part	:Inner_Glow_HandR.part
		Bhvr	:HandR_Offset_02.bhvr
	End

	Event	
		ENAME	ThumbR1_Node
		Type	Local
		At	T1_R	
	End

	Event	
		ENAME	ThumbR1
		Type	Local
		At	T2_R
		part	:Lava_ThumbR_01.part
		Part	:Inner_Glow_ThumbR_01.part
		Bhvr	:ThumbR_01_Offset.bhvr
		Pmagnet ThumbR1_Node
		POther  ThumbR1_Node
	End

	Event	
		ENAME	ThumbR2
		Type	Local
		At	T1_R
		part	:Lava_ThumbR_02.part
		Part	:Inner_Glow_ThumbR_02.part
		Bhvr	:ThumbR_02_Offset.bhvr
	End

	Event	
		ENAME	IndexR1
		Type	Local
		At	Rwing2
		part	:Lava_IndexR_01.part
		Part	:Inner_Glow_IndexR_01.part
		Bhvr	:IndexR1_Offset.bhvr
	End

	Event	
		ENAME	IndexR2
		Type	Local
		At	Rwing1
		part	:Lava_IndexR_02.part
		Part	:Inner_Glow_IndexR_02.part
		Bhvr	:IndexR1_Offset.bhvr
	End

	Event	
		ENAME	MiddleR1
		Type	Local
		At	RingR
		part	:Lava_IndexR_01.part
		Part	:Inner_Glow_IndexR_01.part
		Bhvr	:MiddleR1_Offset.bhvr
	End

	Event	
		ENAME	MiddleR2
		Type	Local
		At	F2_R
		part	:Lava_IndexR_02.part
		Part	:Inner_Glow_IndexR_02.part
		Bhvr	:MiddleR1_Offset.bhvr
	End

	Event	
		ENAME	PinkyR1
		Type	Local
		At	RingR
		part	:Lava_IndexR_01.part
		Part	:Inner_Glow_IndexR_01.part
		Bhvr	:PinkyR1_Offset.bhvr
	End

	Event	
		ENAME	PinkyR2
		Type	Local
		At	F2_R
		part	:Lava_IndexR_02.part
		Part	:Inner_Glow_IndexR_02.part
		Bhvr	:PinkyR1_Offset.bhvr
	End



####################  UArmL  ###########################

	Event	
		ENAME	LArmL_Node
		Type	Local
		At	LArmL	
	End

	Event	
		ENAME	UArmL
		Type	Local
		At	UArmL
		part	:Lava_UArmL_01.part
		part	:Inner_Glow_UArmL_01.part
		Pmagnet LArmL_Node
		POther  LArmL_Node
	End

####################  LArmL  ###########################

	Event	
		ENAME	Hand_L_Node
		Type	Local
		At	HandL	
	End

	Event	
		ENAME	LArmL
		Type	Local
		At	LArmL
		part	:Lava_LArmL_01.part
		part	:Inner_Glow_LArmL_01.part
		Pmagnet Hand_L_Node
		POther  Hand_L_Node
	End

####################  HandL  ###########################

	Event	
		ENAME	HandL
		Type	Local
		At	HandL
		part	:Lava_HandL.part
		Part	:Inner_Glow_HandL.part
		Bhvr	:HandL_Offset_01.bhvr
	End

	Event	
		ENAME	HandL
		Type	Local
		At	HandL
		part	:Lava_HandL.part
		Part	:Inner_Glow_HandL.part
		Bhvr	:HandL_Offset_02.bhvr
	End

	Event	
		ENAME	ThumbL1_Node
		Type	Local
		At	T1_L	
	End

	Event	
		ENAME	ThumbL1
		Type	Local
		At	T2_L
		part	:Lava_ThumbR_01.part
		Part	:Inner_Glow_ThumbR_01.part
		Bhvr	:ThumbR_01_Offset.bhvr
		Pmagnet ThumbL1_Node
		POther  ThumbL1_Node
	End

	Event	
		ENAME	ThumbL2
		Type	Local
		At	T1_L
		#part	:Lava_ThumbL_02.part
		Part	:Inner_Glow_ThumbL_02.part
		Bhvr	:ThumbL_02_Offset.bhvr
	End

	Event	
		ENAME	IndexL1
		Type	Local
		At	Lwing2
		part	:Lava_IndexL_01.part
		Part	:Inner_Glow_IndexL_01.part
		Bhvr	:IndexR1_Offset.bhvr
	End

	Event	
		ENAME	IndexL2
		Type	Local
		At	Lwing1
		part	:Lava_IndexL_02.part
		Part	:Inner_Glow_IndexL_02.part
		Bhvr	:IndexR1_Offset.bhvr
	End

	Event	
		ENAME	MiddleL1
		Type	Local
		At	RingL
		part	:Lava_IndexL_01.part
		Part	:Inner_Glow_IndexL_01.part
		Bhvr	:MiddleR1_Offset.bhvr
	End

	Event	
		ENAME	MiddleL2
		Type	Local
		At	F2_L
		part	:Lava_IndexL_02.part
		Part	:Inner_Glow_IndexL_02.part
		Bhvr	:MiddleR1_Offset.bhvr
	End

	Event	
		ENAME	PinkyL1
		Type	Local
		At	RingL
		part	:Lava_IndexL_01.part
		Part	:Inner_Glow_IndexL_01.part
		Bhvr	:PinkyR1_Offset.bhvr
	End

	Event	
		ENAME	PinkyL2
		Type	Local
		At	F2_L
		part	:Lava_IndexL_02.part
		Part	:Inner_Glow_IndexL_02.part
		Bhvr	:PinkyR1_Offset.bhvr
	End
End
