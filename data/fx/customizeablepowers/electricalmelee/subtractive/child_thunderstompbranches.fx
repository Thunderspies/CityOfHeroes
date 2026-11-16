#############################################################
## Thunder_Stomp_Branches.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Ename	B1
		Type	startpositonly
		At	origin
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch_Root.bhvr
		lifespan 100
	End

	Event
		Ename	B2
		Type	local
		At	B1
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End

	Event
		Ename	B3
		Type	local
		At	B2
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End

	Event
		Ename	B4
		Type	local
		At	B3
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End

	Event
		Ename	B5
		Type	local
		At	B4
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End

	Event
		Ename	B6
		Type	local
		At	B5
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End

	Event
		Ename	B7
		Type	local
		At	B6
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End

	Event
		Ename	B8
		Type	local
		At	B7
		Bhvr	ENEMYFX\V_Diable_Demons\Thunder_Branch.bhvr
		lifespan 100
	End
End

################Static locs######################

Condition
	On 	time
	Time 	3

	Event
		Ename	X1
		Type	start
		At	b1
		lifespan 100
	End

	Event
		Ename	X2
		Type	start
		At	B2
		lifespan 100
		pother  X1
	End

	Event
		Ename	X3
		Type	start
		At	B3
		lifespan 100
		pother  X2
	End

	Event
		Ename	X4
		Type	start
		At	B4
		lifespan 100
		pother  X3
	End

	Event
		Ename	X5
		Type	start
		At	B5
		lifespan 100
		pother  X4
	End

	Event
		Ename	X6
		Type	start
		At	B6
		lifespan 100
		pother  X5
	End

	Event
		Ename	X7
		Type	start
		At	B7
		lifespan 100
		pother  X6
	End

	Event
		Ename	X8
		Type	start
		At	B8
		lifespan 100
		pother  X7
	End
end

################Particle cascade######################

Condition
	On 	time
	Time 	3

	Event
		Type	start
		At	X2
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X1
	End
End

Condition
	On 	time
	Time 	5

	Event
		Type	start
		At	X3
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X2
	End
End

Condition
	On 	time
	Time 	7

	Event
		Type	start
		At	X4
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X3
	End
End

Condition
	On 	time
	Time 	9

	Event
		Type	start
		At	X5
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X4
	End
End

Condition
	On 	time
	Time 	11

	Event
		Type	start
		At	X6
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X5
	End
End

Condition
	On 	time
	Time 	13

	Event
		Type	start
		At	X7
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X6
	End
End

Condition
	On 	time
	Time 	15

	Event
		Type	start
		At	X8
		lifespan 31
		ChildFX	:Child_ThunderStompArcs.fx
		part	:Thunder_Branch.part
		pother  X7
	End
End

#############################################################

End