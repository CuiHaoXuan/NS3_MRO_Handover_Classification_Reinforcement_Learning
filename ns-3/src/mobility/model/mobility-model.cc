/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include <cmath>

#include "mobility-model.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MobilityModel);

TypeId 
MobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MobilityModel")
    .SetParent<Object> ()
    .SetGroupName ("Mobility")
    .AddAttribute ("Position", "The current position of the mobility model.",
                   TypeId::ATTR_SET | TypeId::ATTR_GET,
                   VectorValue (Vector (0.0, 0.0, 0.0)),
                   MakeVectorAccessor (&MobilityModel::SetPosition,
                                       &MobilityModel::GetPosition),
                   MakeVectorChecker ())
    .AddAttribute ("Velocity", "The current velocity of the mobility model.",
                   TypeId::ATTR_GET,
                   VectorValue (Vector (0.0, 0.0, 0.0)), // ignored initial value.
                   MakeVectorAccessor (&MobilityModel::GetVelocity),
                   MakeVectorChecker ())
    .AddTraceSource ("CourseChange", 
                     "The value of the position and/or velocity vector changed",
                     MakeTraceSourceAccessor (&MobilityModel::m_courseChangeTrace),
                     "ns3::MobilityModel::CourseChangeTracedCallback")
    .AddAttribute("IsUE",
    		"Determine this mobility entity is for UE or eNB, aiming at specific pathloss model",
    		BooleanValue(true),
    		MakeBooleanAccessor(&MobilityModel::m_isUE),
    		MakeBooleanChecker())
    .AddAttribute("NodeId",
    		"NodeId is IMSI if the Node is UE, else it is cellId",
    		UintegerValue(0),
    		MakeUintegerAccessor(&MobilityModel::m_nodeId),
    		MakeUintegerChecker<uint64_t>())
  ;
  return tid;
}

MobilityModel::MobilityModel ()
{
}

MobilityModel::~MobilityModel ()
{
}

Vector
MobilityModel::GetPosition (void) const
{
  return DoGetPosition ();
}
Vector
MobilityModel::GetVelocity (void) const
{
  return DoGetVelocity ();
}

void 
MobilityModel::SetPosition (const Vector &position)
{
  DoSetPosition (position);
}

double 
MobilityModel::GetDistanceFrom (Ptr<const MobilityModel> other) const
{
  Vector oPosition = other->DoGetPosition ();
  Vector position = DoGetPosition ();
  return CalculateDistance (position, oPosition);
}

double
MobilityModel::GetAngleFrom(Ptr<const MobilityModel> other) const //THANG
{
	Vector position = DoGetPosition ();
	Vector oPosition = other->DoGetPosition ();
	double dy = oPosition.y-position.y;
	double dx = oPosition.x-position.x;
	double angle = std::atan2(dy,dx);
	return angle; //[-PI,PI]
}

double
MobilityModel::GetRelativeSpeed (Ptr<const MobilityModel> other) const
{
  double x = GetVelocity().x - other->GetVelocity().x;
  double y = GetVelocity().y - other->GetVelocity().y;
  double z = GetVelocity().z - other->GetVelocity().z;
  return sqrt( (x*x) + (y*y) + (z*z) );
}

void
MobilityModel::NotifyCourseChange (void) const
{
  m_courseChangeTrace (this);
}

int64_t
MobilityModel::AssignStreams (int64_t start)
{
  return DoAssignStreams (start);
}

// Default implementation does nothing
int64_t
MobilityModel::DoAssignStreams (int64_t start)
{
  return 0;
}

/*
 * Added by THANG
 */
bool
MobilityModel::GetNodeType(){
	return m_isUE;
}

uint64_t
MobilityModel::GetNodeId(){
	return m_nodeId;
}


} // namespace ns3
