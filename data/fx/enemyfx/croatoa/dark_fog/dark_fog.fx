#########################################################
##	template

FxInfo

Flags    InheritAlpha
#########################################################

Condition
	On 	Time
	Time 	0	

####  OFFSETS UPPER ####

	Event
		EName 	Offset_Base
		Type	local
		At	Hips
		Bhvr	:Offset_Body_Base.bhvr
	End	

	Event
		EName 	Offset_MidR
		Type	local
		At	Offset_Base
		Bhvr	:Offset_Body_MidR.bhvr
	End	

	Event
		EName 	Offset_MidL
		Type	local
		At	Offset_Base
		Bhvr	:Offset_Body_MidL.bhvr
	End	

	Event
		EName 	Offset_MidR2
		Type	local
		At	Offset_Base
		Bhvr	:Offset_Body_MidR2.bhvr
	End	

	Event
		EName 	Offset_MidL2
		Type	local
		At	Offset_Base
		Bhvr	:Offset_Body_MidL2.bhvr
	End	

	Event
		EName 	Offset_MidR3
		Type	local
		At	Offset_Base
		Bhvr	:Offset_Body_MidR3.bhvr
	End	

	Event
		EName 	Offset_MidL3
		Type	local
		At	Offset_Base
		Bhvr	:Offset_Body_MidL3.bhvr
	End	

####  OFFSETS LOWER ####

	Event
		EName 	Offset_LowerR
		Type	local
		At	Hips
		Bhvr	:Offset_Body_MidR.bhvr
	End	

	Event
		EName 	Offset_LowerL
		Type	local
		At	Hips
		Bhvr	:Offset_Body_MidL.bhvr
	End	

	Event
		EName 	Offset_LowerR2
		Type	local
		At	Hips
		Bhvr	:Offset_Body_MidR2.bhvr
	End	

	Event
		EName 	Offset_LowerL2
		Type	local
		At	Hips
		Bhvr	:Offset_Body_MidL2.bhvr
	End	

	Event
		EName 	Offset_LowerR3
		Type	local
		At	Hips
		Bhvr	:Offset_Body_MidR3.bhvr
	End	

	Event
		EName 	Offset_LowerL3
		Type	local
		At	Hips
		Bhvr	:Offset_Body_MidL3.bhvr
	End	

####  PARTICLES ####

	Event
		EName 	Core_01
		Type	local
		At	Offset_Base
		Part	:Core_Fog_01.part
	End	

	Event
		EName 	Body_01
		Type	local
		At	Offset_MidR
		Part	:Body_Fog_01.part
	End	

	Event
		EName 	Body_02
		Type	local
		At	Offset_MidL
		Part	:Body_Fog_01.part
	End	

	Event
		EName 	Body_03
		Type	local
		At	Offset_MidR2
		Part	:Body_Fog_01.part
	End	

	Event
		EName 	Body_04
		Type	local
		At	Offset_MidL2
		Part	:Body_Fog_01.part
	End	

	Event
		EName 	Body_05
		Type	local
		At	Offset_MidR3
		Part	:Body_Fog_01.part
	End	

	Event
		EName 	Body_06
		Type	local
		At	Offset_MidL3
		Part	:Body_Fog_01.part
	End	

####  PARTICLES LOWER####

	Event
		EName 	Body_01
		Type	local
		At	Offset_LowerR
		Part	:Lower_Fog_01.part
	End	

	Event
		EName 	Body_02
		Type	local
		At	Offset_LowerL
		Part	:Lower_Fog_01.part
	End	

	Event
		EName 	Body_03
		Type	local
		At	Offset_LowerR2
		Part	:Lower_Fog_01.part
	End	

	Event
		EName 	Body_04
		Type	local
		At	Offset_LowerL2
		Part	:Lower_Fog_01.part
	End	

	Event
		EName 	Body_05
		Type	local
		At	Offset_LowerR3
		Part	:Lower_Fog_01.part
	End	

	Event
		EName 	Body_06
		Type	local
		At	Offset_LowerL3
		Part	:Lower_Fog_01.part
	End	


End


