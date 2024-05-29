//! \file flow_solver.cpp
#include "opensd/flow_solver.h"

// #include <algorithm>
// #include <cmath>
#include <iostream>
// #include <Eigen/Dense>

#include "opensd/circuit.h"

namespace opensd {

//==============================================================================
// Global variables
//==============================================================================

// Define the nonlinear function for the face's momentum equation
struct FaceFunctor {
  using Scalar = double;
  using InputType = Eigen::VectorXd;
  using ValueType = Eigen::VectorXd;
  using JacobianType = Eigen::MatrixXd;
  enum { InputsAtCompileTime = 1, ValuesAtCompileTime = 1 };  
  
  // Constructor to initialize with parameters
  FaceFunctor(double time, double delt, bool trans_sim, double alpha_mom, PFace* face)
    : time(time), delt(delt), trans_sim(trans_sim), alpha_mom(alpha_mom), face(face) {}

  // Function to be solved: returns f(x)
  int operator()(const Eigen::VectorXd &x, Eigen::VectorXd &fvec) const {
    // Replace with the actual momentum equation
    double vflow_gues = x(0);
    fvec(0) = face->eqn_mom(vflow_gues,time,delt,trans_sim,alpha_mom);/* momentum equation involving vflow_gues and other parameters */;
    return 0;
  }

/* 
  // Optionally provide the Jacobian
  int df(const Eigen::VectorXd &x, Eigen::MatrixXd &fjac) const {
    // Derivative of the momentum equation with respect to vflow_gues
    fjac(0, 0) = // derivative of momentum equation ;
    return 0;
  }

 */
  int inputs() const { return 1; }
  int values() const { return 1; }

private:
  double time;
  double delt;
  bool trans_sim;
  double alpha_mom;
  PFace* face;
};

//==============================================================================
// Non-member functions
//==============================================================================

void exec_massmom(double time, double delt, bool trans_sim, double alpha_mom, int main_iter, int flow_iter) {
  
   for (auto& circuit : model::circuits) {
    // if (!trans_sim && !circuit.solveSS) continue;
    // std::cout << circuit.identifier << std::endl;
    // for (auto& branch : circuit.branches) { // Guess flow rate calculation
    for (auto& pipe : circuit.pipes) { 
      // branch.choked = false;
      // for (auto face = branch.faces.rbegin(); face != branch.faces.rend(); ++face) { // Reverse iteration
      for (auto& it : pipe.faces) { // Reverse iteration
        auto& face = *it;
        // face.choked = false;
        // if (circuit.fllib == "CoolProp" && circuit.flname != "Air" && circuit.flname != "Nitrogen") {
          // face.update_Gcr();
        // }
        // if (!branch.choked && face.dnode->spres_gues < face.pcr && dynamic_cast<turbo_comp::Turbine*>(&face) == nullptr) {
          // branch.choked = true;
          // face.choked = true;
          // face.spres_gues = face.pcr;
          // face.stemp_gues = face.ther_cr.T();
          // face.ther_gues.update(face.ther_cr);
          // face.velocity = face.Gcr / face.ther_gues.rhomass();
          // face.tpres_gues = face.spres_gues + 0.5 * face.ther_gues.rhomass() * std::pow(face.velocity, 2);
          // face.ttemp_gues = face.stemp_gues + 0.5 * std::pow(face.velocity, 2) / face.ther_gues.cpmass();
          // for (int i = 0; i < 100; ++i) {
            // face.unode->update_staticvar();
            // face.unode->ther_gues.update(CoolProp::HmassP_INPUTS, face.unode->senth_gues, face.unode->spres_gues);
            // face.dnode->update_staticvar();
            // face.dnode->ther_gues.update(CoolProp::HmassP_INPUTS, face.dnode->senth_gues, face.dnode->spres_gues);
          // }
          // face.G = face.Gcr;
          // face.vflow_gues = face.G * face.cfarea * face.opening / face.ther_gues.rhomass();
        // } else {

          Eigen::VectorXd x(1);
          x(0) = face.vflow_gues;

          FaceFunctor functor(time, delt, trans_sim, alpha_mom, &face);
          Eigen::NumericalDiff<FaceFunctor> numDiff(functor);
          Eigen::LevenbergMarquardt<Eigen::NumericalDiff<FaceFunctor>> lm(numDiff);

          int info = lm.minimize(x);
          face.vflow_gues = x(0);

          // if (face.opening == 0.0) continue;
          // if (branch.isolated && !trans_sim) {
            // face.vflow_gues = 0.0;
            // continue;
          // }
          if (std::abs(face.vflow_gues) < 1.E-8 && main_iter == 0) { // Tune the value 1.E-8 as needed
            face.vflow_gues = 1.E-8 * std::copysign(1.0, face.vflow_gues);
            if (face.vflow_gues == 0.0) {
              face.vflow_gues = 1.E-8;
            }
          }
          // std::cout << face.vflow_gues << std::endl;
          // if (dynamic_cast<PFace*>(&face) != nullptr || dynamic_cast<or_comp::Orifice*>(&face) != nullptr) {
            // face.G = face.vflow_gues * face.ther_gues.rhomass() / (face.cfarea * face.opening);
          // }
        // }
        face.update_abcoef(time, delt, trans_sim, alpha_mom);
      }
    }

/*
    // Pressure corrections
    int n = circuit.nodes.size();
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(n);
    for (int i = 0; i < n; ++i) {
      auto& node = circuit.nodes[i];
      if (node.flowreg == "Slug") continue;
      double B, D;
      if (node.ther_old.phase() == 6) {
        B = node.B1 + node.volume * node.ther_old.first_two_phase_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass) / node.ther_old.rhomass();
        A(i, i) = trans_sim * B * node.ther_old.rhomass() / delt;
        D = trans_sim * node.volume * node.ther_old.first_two_phase_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
      } else {
        B = node.B1 + node.volume * node.ther_old.first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass) / node.ther_old.rhomass();
        A(i, i) = trans_sim * B * node.ther_old.rhomass() / delt;
        D = trans_sim * node.volume * node.ther_old.first_partial_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
      }
      b(i) = -trans_sim * B * node.ther_old.rhomass() / delt * (node.tpres_gues - node.ther_gues.rhomass() * std::pow(node.velocity, 2) / 2.0 - node.spres_old)
           - trans_sim * D * (node.senth_gues - node.senth_old) / delt;

      for (auto& iface : node.ifaces) {
        A(i, iface->unode->node_ind) = -alpha_mom * (iface->aminus * iface->ther_gues.rhomass() + iface->bminus * iface->vflow_gues);
        A(i, i) -= alpha_mom * (-iface->aplus * iface->ther_gues.rhomass() + iface->bplus * iface->vflow_gues);
        b(i) += alpha_mom * (iface->ther_gues.rhomass() * iface->vflow_gues) + (1.0 - alpha_mom) * (iface->ther_old.rhomass() * iface->vflow_old);
        if (A(i, iface->unode->node_ind) > 0.0) {
          if ((show_warn && trans_sim) || !trans_sim) {
            std::cout << "Warning: upstream coef negative. " << node.identifier << std::endl;
          }
        }
      }
      for (auto& oface : node.ofaces) {
        A(i, oface->dnode->node_ind) = -alpha_mom * (oface->aplus * oface->ther_gues.rhomass() - oface->bplus * oface->vflow_gues);
        A(i, i) += alpha_mom * (oface->aminus * oface->ther_gues.rhomass() + oface->bminus * oface->vflow_gues);
        b(i) -= alpha_mom * (oface->ther_gues.rhomass() * oface->vflow_gues) - (1.0 - alpha_mom) * (oface->ther_old.rhomass() * oface->vflow_old);
        if (A(i, oface->dnode->node_ind) > 1.E-6) { // Pending check if 0
          if ((show_warn && trans_sim) || !trans_sim) {
            std::cout << "Warning: downstream coef negative. " << node.identifier << " " << oface->dnode->identifier << " " << A(i, oface->dnode->node_ind) << std::endl;
          }
        }
      }

      if (node.fixed_var.count("P") > 0 && dynamic_cast<cont::Reservoir*>(&node) == nullptr) {
        node.msource = -b(i);
      }
      if (A(i, i) < -1.E-6) { // Pending check if 0
        if ((show_warn && trans_sim) || !trans_sim) {
          std::cout << "Warning: negative A coef. " << node.identifier << " " << A(i, i) << std::endl;
        }
      }
    }

    for (int i = 0; i < n; ++i) {
      auto& node = circuit.nodes[i];
      if (node.fixed_var.count("msource") > 0) {
        b(i) += node.msource;
      }
    }

    // Deleting boundary condition rows and columns
    for (auto ind : circuit.Pbound_ind) {
      A.row(ind).setZero();
      A.col(ind).setZero();
      b(ind) = 0.0;
    }

    Eigen::VectorXd pc;
    if (A.fullPivLu().isInvertible()) {
      pc = A.lu().solve(b);
    } else {
      pc = Eigen::VectorXd::Zero(b.size()); // Skipping pressure correction for infinite conditional number (to be verified)
    }
    
    // Insert zeros at boundary indices
    for (auto ind : circuit.Pbound_ind) {
      pc(ind) = 0.0;
    }

    // Flow rate corrections
    for (auto& face : circuit.faces) {
      if (!face->choked) {
        double vc = face->aminus * pc(face->unode->node_ind) - face->aplus * pc(face->dnode->node_ind);
        face->vflow_gues += vc;
      }
      face->update_velocity();
    }

    // Pressure and density corrections
    for (int i = 0; i < n; ++i) {
      auto& node = circuit.nodes[i];
      if (node.flowreg == "Slug") continue;
      double relax = 0.6;
      if (solver::relax_pres) {
        relax = solver::relax_pres;
      }
      node.tpres_gues += relax * pc(i);
      if (node.tpres_gues < 0.0) {
        std::cerr << "Negative tpres " << node.identifier << " " << node.tpres_gues << " " << node.tpres_old << std::endl;
        std::cerr << pc << std::endl;
        std::exit(EXIT_FAILURE);
      }

      node.update_staticvar();

      if (main_iter == 0) node.pc_flag = false;
      node.ther_gues.update(CoolProp::HmassP_INPUTS, node.senth_gues, node.spres_gues);
      if (circuit.flag_tp || dynamic_cast<cont::TPTank*>(&node) != nullptr) {
        node.ther_gues.update_sat();
      }
      if (trans_sim && (node.pc_flag || node.ther_gues.phase() != node.ther_old.phase())) {
        node.pc_flag = true;
      }
    }

    for (auto& face : circuit.faces) {
      if (!face->choked) {
        face->update_statevar();
        face->ther_gues.update();
        if (circuit.flag_tp) face->ther_gues.update_sat();
        face->update_heat_input(time, delt);
        face->update_fricfact();
      } else {
        face->update_Gcr();
        face->G = std::copysign(face->Gcr, face->vflow_gues);
        face->vflow_gues = face->G * face->cfarea / face->ther_gues.rhomass();
      }
    }
 */
  }
}

}